/**
 * @file QueueTest.cpp
 * @author ayano
 * @date 1/23/24
 * @brief
*/

#include "Queue.hpp"
#include "IMessage.hpp"
#include "gtest/gtest.h"
#include <thread>
#include <string>
#include <chrono>

using namespace messaging;

TEST(QueueTest, HighDataFlowWithConcurrentAccess) {
    Queue<IntMessage> queue;
    const int dataFlow = 10000;
    std::vector<IntMessage> messages;
    for (int i = 0; i < dataFlow; ++i) {
        messages.push_back(IntMessage(i));
    }

    std::thread producer([&]() {
        for (const auto& message : messages) {
            queue.push(message);
        }
    });

    std::vector<IntMessage> fetchedMessages;
    std::thread consumer([&]() {
        for (int i = 0; i < dataFlow; ++i) {
            fetchedMessages.push_back(*queue.wait());
        }
    });

    producer.join();
    consumer.join();

    ASSERT_EQ(fetchedMessages.size(), dataFlow);
    for (int i = 0; i < dataFlow; ++i) {
        ASSERT_EQ(fetchedMessages[i].getContent(), i);
    }
}

TEST(QueueTest, HighDataFlowWithMultipleProducersAndConsumers) {
    Queue<IntMessage> queue;
    const int dataFlow = 10000;
    std::vector<IntMessage> messages;
    for (int i = 0; i < dataFlow; ++i) {
        messages.push_back(IntMessage(i));
    }

    std::thread producer1([&]() {
        for (const auto& message : messages) {
            queue.push(message);
        }
    });

    std::thread producer2([&]() {
        for (const auto& message : messages) {
            queue.push(message);
        }
    });

    std::vector<IntMessage> fetchedMessages1;
    std::thread consumer1([&]() {
        for (int i = 0; i < dataFlow; ++i) {
            fetchedMessages1.push_back(*queue.wait());
        }
    });

    std::vector<IntMessage> fetchedMessages2;
    std::thread consumer2([&]() {
        for (int i = 0; i < dataFlow; ++i) {
            fetchedMessages2.push_back(*queue.wait());
        }
    });

    producer1.join();
    producer2.join();
    consumer1.join();
    consumer2.join();

    ASSERT_EQ(fetchedMessages1.size() + fetchedMessages2.size(), 2 * dataFlow);
}
