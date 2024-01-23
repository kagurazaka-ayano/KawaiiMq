/**
 * @file ManagerTest.cpp
 * @author ayano
 * @date 1/23/24
 * @brief
*/

#include "Queue.hpp"
#include "IMessage.h"
#include "MessageQueueManager.hpp"
#include "gtest/gtest.h"
#include <thread>
#include <string>
#include <chrono>
#include <iostream>

TEST(managerTest, SubscribeAndMessage) {
    auto topic1 = messaging::Topic("topic1");
    auto topic2 = messaging::Topic("topic2");
    auto queue11 = messaging::Queue<messaging::IntMessage>("test11");
    auto queue12 = messaging::Queue<messaging::IntMessage>("test12");
    auto queue21 = messaging::Queue<messaging::IntMessage>("test21");
    auto queue22 = messaging::Queue<messaging::IntMessage>("test22");

    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    manager->subscribe(topic1, queue11);
    manager->subscribe(topic1, queue12);
    manager->subscribe(topic2, queue21);
    manager->subscribe(topic2, queue22);
    int val, ret;
    bool finished = false;
    for(int i = 0; i < 10; ++i) {
        val = i;
        manager->publish(messaging::IntMessage(val), topic1);
        ret = manager->retrieve(queue11)->getContent();
        EXPECT_EQ(ret, val);
        ret = manager->retrieve(queue12)->getContent();
        EXPECT_EQ(ret, val);
        val = i + 1;
        manager->publish(messaging::IntMessage(val), topic1);
        ret = manager->retrieve(queue11)->getContent();
        EXPECT_EQ(ret, val);
        ret = manager->retrieve(queue12)->getContent();
        EXPECT_EQ(ret, val);
        val = i;
        manager->publish(messaging::IntMessage(val), topic2);
        ret = manager->retrieve(queue21)->getContent();
        EXPECT_EQ(ret, val);
        ret = manager->retrieve(queue22)->getContent();
        EXPECT_EQ(ret, val);
        val = i + 1;
        manager->publish(messaging::IntMessage(val), topic2);
        ret = manager->retrieve(queue21)->getContent();
        EXPECT_EQ(ret, val);
        ret = manager->retrieve(queue22)->getContent();
        EXPECT_EQ(ret, val);
    }
}

auto queue11 = messaging::Queue<messaging::IntMessage>("test11");
auto queue12 = messaging::Queue<messaging::IntMessage>("test12");
auto queue21 = messaging::Queue<messaging::IntMessage>("test21");
auto queue22 = messaging::Queue<messaging::IntMessage>("test22");

void pusher(const messaging::Topic& topic1, const messaging::Topic& topic2) {
    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    int val;
    for(int i = 0; i < 10; ++i) {
        val = i;
        manager->publish(messaging::IntMessage(val), topic1);
        val = i + 1;
        manager->publish(messaging::IntMessage(val), topic1);
        val = i;
        manager->publish(messaging::IntMessage(val), topic2);
        val = i + 1;
        manager->publish(messaging::IntMessage(val), topic2);
    }
}

void fetcher() {
    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    int ret, val = 0;
    while(!(queue11.empty() && queue12.empty() && queue21.empty() && queue22.empty())) {
        ret = manager->retrieve(queue11)->getContent();
        EXPECT_EQ(ret, val);
        ret = manager->retrieve(queue12)->getContent();
        EXPECT_EQ(ret, val);
        ret = manager->retrieve(queue11)->getContent();
        EXPECT_EQ(ret, val + 1);
        ret = manager->retrieve(queue12)->getContent();
        EXPECT_EQ(ret, val + 1);
        ret = manager->retrieve(queue21)->getContent();
        EXPECT_EQ(ret, val);
        ret = manager->retrieve(queue22)->getContent();
        EXPECT_EQ(ret, val);
        ret = manager->retrieve(queue21)->getContent();
        EXPECT_EQ(ret, val + 1);
        ret = manager->retrieve(queue22)->getContent();
        EXPECT_EQ(ret, val + 1);
        val++;
    }
}

TEST(managerTest, multiThreadPush) {
    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    auto topic1 = messaging::Topic("topic1");
    auto topic2 = messaging::Topic("topic2");
    manager->subscribe(topic1, queue11);
    manager->subscribe(topic1, queue12);
    manager->subscribe(topic2, queue21);
    manager->subscribe(topic2, queue22);
    auto th1 = std::thread(fetcher);
    auto th2 = std::thread(pusher, std::ref(topic1), std::ref(topic2));
    th1.join();
    th2.join();
}

TEST(managerTest, asyncPublish) {
    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    auto topic1 = messaging::Topic("topic1");
    auto topic2 = messaging::Topic("topic2");
    manager->subscribe(topic1, queue11);
    manager->subscribe(topic1, queue12);
    manager->subscribe(topic2, queue21);
    manager->subscribe(topic2, queue22);
    int worker_count = 2;
    std::thread publisher;
    std::vector<std::thread> receiver;
    auto th = std::thread(pusher, std::ref(topic1), std::ref(topic2));

    th.join();
    receiver.reserve(worker_count);
    for(int i = 0; i < worker_count; i++) {
        receiver.emplace_back(fetcher);
    }
    for(auto& i : receiver) {
        i.join();
    }

}
