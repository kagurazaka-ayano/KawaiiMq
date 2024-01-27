/**
 * @file ManagerTest.cpp
 * @author ayano
 * @date 1/23/24
 * @brief
*/

#include "kawaiiMQ.h"
#include "gtest/gtest.h"
#include <string>
#include <thread>
#include <future>
#include <iostream>

TEST(ProducerTest, PublishMessageToSingleTopic) {
    auto topic = messaging::Topic("topic");
    auto queue = messaging::Queue<messaging::IntMessage>("test");
    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    manager->relate(topic, queue);
    auto producer = messaging::Producer<messaging::IntMessage>();
    producer.publishMessage(topic, messaging::IntMessage(1));
    EXPECT_FALSE(queue.empty());
}

TEST(ProducerTest, PublishMessageToMultipleTopics) {
    auto topic1 = messaging::Topic("topic1");
    auto topic2 = messaging::Topic("topic2");
    auto queue1 = messaging::Queue<messaging::IntMessage>("test1");
    auto queue2 = messaging::Queue<messaging::IntMessage>("test2");
    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    manager->relate(topic1, queue1);
    manager->relate(topic2, queue2);
    auto producer = messaging::Producer<messaging::IntMessage>();
    producer.publishMessage(topic1, messaging::IntMessage(1));
    producer.publishMessage(topic2, messaging::IntMessage(2));
    EXPECT_FALSE(queue1.empty());
    EXPECT_FALSE(queue2.empty());
}


TEST(ProducerTest, PublishMessageToUnrelatedTopic) {
    auto topic = messaging::Topic("topic");
    auto queue = messaging::Queue<messaging::IntMessage>("test");
    auto producer = messaging::Producer<messaging::IntMessage>();
    producer.publishMessage(topic, messaging::IntMessage(1));
    EXPECT_TRUE(queue.empty());
}
