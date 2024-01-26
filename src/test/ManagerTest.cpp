/**
 * @file ManagerTest.cpp
 * @author ayano
 * @date 1/23/24
 * @brief
*/

#include "kawaiiMQ.h"
#include <string>
#include <thread>
#include <chrono>
#include <iostream>

TEST(operatorTest, SubscribeAndMessage) {
    auto topic1 = messaging::Topic("topic1");
    auto topic2 = messaging::Topic("topic2");
    auto queue11 = messaging::Queue<messaging::IntMessage>("test11");
    auto queue12 = messaging::Queue<messaging::IntMessage>("test12");
    auto queue21 = messaging::Queue<messaging::IntMessage>("test21");
    auto queue22 = messaging::Queue<messaging::IntMessage>("test22");

    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    manager->relate(topic1, queue11);
    manager->relate(topic1, queue12);
    manager->relate(topic2, queue21);
    manager->relate(topic2, queue22);
    int val, ret;
    bool finished = false;
    auto op = messaging::Client();
    op.subscribe(topic1);
    op.subscribe(topic2);
    for(int i = 0; i < 10; ++i) {
        val = i;
        op.publishMessage(messaging::IntMessage(val));
        auto ans = op.fetchSingleTopic<messaging::IntMessage>(topic1);
        ret = ans[0]->getContent();
        EXPECT_EQ(ret, val);
        ret = ans[1]->getContent();
        EXPECT_EQ(ret, val);
        ans = op.fetchSingleTopic<messaging::IntMessage>(topic2);
        ret = ans[0]->getContent();
        EXPECT_EQ(ret, val);
        ret = ans[1]->getContent();
        EXPECT_EQ(ret, val);
        op.publishMessage(messaging::IntMessage(val + 1));
        auto t = op.fetchMessage<messaging::IntMessage>();
        EXPECT_EQ(t[topic1][0]->getContent(), val + 1);
        EXPECT_EQ(t[topic1][1]->getContent(), val + 1);
        EXPECT_EQ(t[topic2][0]->getContent(), val + 1);
        EXPECT_EQ(t[topic2][1]->getContent(), val + 1);
    }
}

auto queue11 = messaging::Queue<messaging::IntMessage>("test11");
auto queue12 = messaging::Queue<messaging::IntMessage>("test12");
auto queue21 = messaging::Queue<messaging::IntMessage>("test21");
auto queue22 = messaging::Queue<messaging::IntMessage>("test22");

void pusher(const messaging::Topic& topic1, const messaging::Topic& topic2) {
    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    int val;
    auto op = messaging::Client();
    op.subscribe(topic1);
    op.subscribe(topic2);
    for(int i = 0; i < 10; ++i) {
        op.publishMessage(messaging::IntMessage(i));
        op.publishMessage(messaging::IntMessage(i + 1));
    }
}

void fetcher(bool& finish, const messaging::Topic& topic1, const messaging::Topic& topic2) {
    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    int ret, val = 0;
    auto op = messaging::Client();
    op.subscribe(topic1);
    op.subscribe(topic2);
    while(!finish) {
        auto ans = op.fetchSingleTopic<messaging::IntMessage>(topic1);
        ret = ans[0]->getContent();
        EXPECT_EQ(ret, val);
        std::cout << "ret: " << ret << " val: " << val << std::endl;
        ret = ans[1]->getContent();
        EXPECT_EQ(ret, val + 1);
        std::cout << "ret: " << ret << " val: " << val + 1 << std::endl;
        ans = op.fetchSingleTopic<messaging::IntMessage>(topic2);
        ret = ans[0]->getContent();
        EXPECT_EQ(ret, val);
        std::cout << "ret: " << ret << " val: " << val << std::endl;
        ret = ans[1]->getContent();
        EXPECT_EQ(ret, val + 1);
        std::cout << "ret: " << ret << " val: " << val + 1 << std::endl;
        ++val;
    }
}

TEST(operatorTest, Multithread) {
    auto topic1 = messaging::Topic("topic1");
    auto topic2 = messaging::Topic("topic2");
    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    manager->relate(topic1, queue11);
    manager->relate(topic1, queue12);
    manager->relate(topic2, queue21);
    manager->relate(topic2, queue22);
    auto th1 = std::thread(pusher, std::ref(topic1), std::ref(topic2));
    bool finish = false;
    auto th2 = std::thread(fetcher, std::ref(finish), std::ref(topic1), std::ref(topic2));
    th1.join();
    finish = true;
    th2.join();
}

