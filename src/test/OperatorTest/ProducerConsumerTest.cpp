/**
 * @file ManagerTest.cpp
 * @author ayano
 * @date 1/23/24
 * @brief
*/

#include "kawaiiMQ.h"
#include "gtest/gtest.h"
#include <thread>

namespace KawaiiMQ {
    class ProducerTest : public ::testing::Test {
    protected:
        void TearDown() override {
            MessageQueueManager::Instance()->flush();
        }
    };

    TEST_F(ProducerTest, SendAndReceive) {
        Topic topic("testTopic");
        auto queue = makeQueue("testQueue");
        auto m = makeMessage(0);
        MessageQueueManager::Instance()->relate(topic, queue);
        Producer producer("prod");
        producer.subscribe(topic);
        Consumer consumer("cons");
        consumer.subscribe(topic);
        producer.publishMessage(topic, m);
        ASSERT_EQ(queue->size(), 1);
        auto message = consumer.fetchSingleTopic(topic);
        ASSERT_EQ(message[0], m);
    }

    TEST_F(ProducerTest, SubscribeAndUnsubscribe) {
        Topic topic("testTopic");
        auto q = makeQueue("queue");
        MessageQueueManager::Instance()->relate(topic, q);
        Producer producer("prod");
        Consumer consumer("cons");
        producer.subscribe(topic);
        consumer.subscribe(topic);
        EXPECT_THROW(producer.subscribe(topic), TopicException);
        EXPECT_THROW(consumer.subscribe(topic), TopicException);
        producer.unsubscribe(topic);
        consumer.unsubscribe(topic);
        EXPECT_THROW(producer.unsubscribe(topic), TopicException);
        EXPECT_THROW(consumer.unsubscribe(topic), TopicException);
    }

    TEST_F(ProducerTest, ConcurrentProducerConsumer) {
        auto q = makeQueue("queue");
        Topic topic("test");
        MessageQueueManager::Instance()->relate(topic, q);
        Producer producer("prod");
        Consumer consumer("cons");
        producer.subscribe(topic);
        consumer.subscribe(topic);
        std::thread t1([&]() { producer.broadcastMessage(makeMessage(1));});
        std::thread t2([&]() { consumer.fetchMessage();});

        t1.join();
        t2.join();

        ASSERT_EQ(q->size(), 0);
    }


    TEST_F(ProducerTest, MultipleQueuesSingleProducerMultipleConsumers) {
        Topic topic("testTopic");
        auto queue1 = makeQueue("testQueue1");
        auto queue2 = makeQueue("testQueue2");
        auto queue3 = makeQueue("testQueue3");

        MessageQueueManager::Instance()->relate(topic, queue1);
        MessageQueueManager::Instance()->relate(topic, queue2);
        MessageQueueManager::Instance()->relate(topic, queue3);

        Producer producer("prod");
        producer.subscribe(topic);

        Consumer consumer1("cons1");
        consumer1.subscribe(topic);

        Consumer consumer2("cons2");
        consumer2.subscribe(topic);

        Consumer consumer3("cons3");
        consumer3.subscribe(topic);

        std::thread t1([&]() {
            for(int i = 0; i < 3; i++) {
                producer.broadcastMessage(makeMessage(1));
            }
        });

        std::thread t2([&]() { consumer1.fetchMessage(); });
        std::thread t3([&]() { consumer2.fetchMessage(); });
        std::thread t4([&]() { consumer3.fetchMessage(); });

        t1.join();
        t2.join();
        t3.join();
        t4.join();

        ASSERT_EQ(queue1->size(), 0);
        ASSERT_EQ(queue2->size(), 0);
        ASSERT_EQ(queue3->size(), 0);
    }
}
