/**
 * @file ManagerTest.cpp
 * @author ayano
 * @date 1/26/24
 * @brief
*/

#include "kawaiiMQ.h"
#include "gtest/gtest.h"
#include <thread>

namespace KawaiiMQ{

    class MessageQueueManagerTest : public ::testing::Test {
    protected:
        void TearDown() override {
            MessageQueueManager::Instance()->flush();
        }
    };

    TEST_F(MessageQueueManagerTest, SingletonPattern) {
        auto instance1 = MessageQueueManager::Instance();
        auto instance2 = MessageQueueManager::Instance();
        ASSERT_EQ(instance1, instance2);
    }

    TEST_F(MessageQueueManagerTest, RelateAndUnrelate) {
        Topic topic("testTopic");
        auto queue = std::make_shared<Queue>("testQueue");
        auto instance = MessageQueueManager::Instance();
        instance->relate(topic, queue);
        ASSERT_TRUE(instance->isRelated(topic, queue));
        instance->unrelate(topic, queue);
        ASSERT_FALSE(instance->isRelated(topic, queue));
    }

    TEST_F(MessageQueueManagerTest, HighDataFlow) {
        auto instance = MessageQueueManager::Instance();
        for (int i = 0; i < 10000; ++i) {
            Topic topic("testTopic" + std::to_string(i));
            auto queue = makeQueue("testQueue" + std::to_string(i));
            instance->relate(topic, queue);
            ASSERT_TRUE(instance->isRelated(topic, queue));
        }
    }

    TEST_F(MessageQueueManagerTest, Concurrency) {
        auto instance = MessageQueueManager::Instance();
        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back([=]() {
                Topic topic("testTopic" + std::to_string(i));
                auto queue = makeQueue("testQueue" + std::to_string(i));
                instance->relate(topic, queue);
                ASSERT_TRUE(instance->isRelated(topic, queue));
                instance->unrelate(topic, queue);
                ASSERT_FALSE(instance->isRelated(topic, queue));
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }
    }

    TEST_F(MessageQueueManagerTest, ConcurrencyHighDataFlow) {
        auto instance = MessageQueueManager::Instance();
        std::vector<std::thread> threads;
        for (int i = 0; i < 12; ++i) {
            threads.emplace_back([=]() {
                Topic topic("testTopic" + std::to_string(i));
                auto queue = makeQueue("testQueue" + std::to_string(i));
                instance->relate(topic, queue);
                ASSERT_TRUE(instance->isRelated(topic, queue));
                instance->unrelate(topic, queue);
                ASSERT_FALSE(instance->isRelated(topic, queue));
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }
    }
}
