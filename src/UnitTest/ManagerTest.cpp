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
        ret = queue11.wait()->getContent();
        EXPECT_EQ(ret, val);
        ret = queue12.wait()->getContent();
        EXPECT_EQ(ret, val);
        val = i + 1;
        manager->publish(messaging::IntMessage(val), topic1);
        ret = queue11.wait()->getContent();
        EXPECT_EQ(ret, val);
        ret = queue12.wait()->getContent();
        EXPECT_EQ(ret, val);
        val = i;
        manager->publish(messaging::IntMessage(val), topic2);
        ret = queue21.wait()->getContent();
        EXPECT_EQ(ret, val);
        ret = queue22.wait()->getContent();
        EXPECT_EQ(ret, val);
        val = i + 1;
        manager->publish(messaging::IntMessage(val), topic2);
        ret = queue21.wait()->getContent();
        EXPECT_EQ(ret, val);
        ret = queue22.wait()->getContent();
        EXPECT_EQ(ret, val);
    }
}

