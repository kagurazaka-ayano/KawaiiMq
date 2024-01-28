/**
 * @file ManagerTest.cpp
 * @author ayano
 * @date 1/23/24
 * @brief
*/

#include "kawaiiMQ.h"
#include "gtest/gtest.h"
#include <string>

using namespace messaging;

TEST(ProducerTest, SubscribeToNewTopic) {
    Producer<IntMessage> producer;
    Topic topic1{"topic1"};
    ASSERT_NO_THROW(producer.subscribe(topic1));
}

TEST(ProducerTest, SubscribeToAlreadySubscribedTopic) {
    Producer<IntMessage> producer;
    Topic topic1{"topic1"};
    producer.subscribe(topic1);
    ASSERT_THROW(producer.subscribe(topic1), std::runtime_error);
}

TEST(ProducerTest, UnsubscribeFromSubscribedTopic) {
    Producer<IntMessage> producer;
    Topic topic1{"topic1"};
    producer.subscribe(topic1);
    ASSERT_NO_THROW(producer.unsubscribe(topic1));
}

TEST(ProducerTest, UnsubscribeFromNotSubscribedTopic) {
    Producer<IntMessage> producer;
    Topic topic1{"topic1"};
    ASSERT_THROW(producer.unsubscribe(topic1), std::runtime_error);
}

TEST(ProducerTest, PublishMessageToSubscribedTopic) {
    Producer<IntMessage> producer;
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    Topic topic1{"topic1"};
    manager->relate(topic1, queue);
    auto message = IntMessage(1);
    producer.subscribe(topic1);
    ASSERT_NO_THROW(producer.publishMessage(topic1, message));
}

TEST(ProducerTest, PublishMessageToNotSubscribedTopic) {
    Producer<IntMessage> producer;
    Topic topic1{"topic1"};
    auto message = IntMessage(1);
    ASSERT_THROW(producer.publishMessage(topic1, message), std::runtime_error);
}

TEST(ProducerTest, BroadcastMessageWhenNoTopicSubscribed) {
    Producer<IntMessage> producer;
    auto message = IntMessage(1);
    ASSERT_NO_THROW(producer.broadcastMessage(message));
}

TEST(ProducerTest, BroadcastMessageWhenTopicsSubscribed) {
    Producer<IntMessage> producer;
    Topic topic1{"topic1"};
    Topic topic2{"topic2"};
    auto queue1 = Queue<IntMessage>();
    auto queue2 = Queue<IntMessage>();
    auto message = IntMessage(1);
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->relate(topic1, queue1);
    manager->relate(topic2, queue2);
    producer.subscribe(topic1);
    producer.subscribe(topic2);
    ASSERT_NO_THROW(producer.broadcastMessage(message));
}


TEST(ConsumerTest, SubscribeToNewTopic) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    ASSERT_NO_THROW(consumer.subscribe(topic1));
}

TEST(ConsumerTest, SubscribeToAlreadySubscribedTopic) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    consumer.subscribe(topic1);
    ASSERT_THROW(consumer.subscribe(topic1), std::runtime_error);
}

TEST(ConsumerTest, UnsubscribeFromSubscribedTopic) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    consumer.subscribe(topic1);
    ASSERT_NO_THROW(consumer.unsubscribe(topic1));
}

TEST(ConsumerTest, UnsubscribeFromNotSubscribedTopic) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    ASSERT_THROW(consumer.unsubscribe(topic1), std::runtime_error);
}

TEST(ConsumerTest, FetchMessageFromSubscribedTopic) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    consumer.subscribe(topic1);
    ASSERT_NO_THROW(consumer.fetchSingleTopic(topic1));
}

TEST(ConsumerTest, FetchMessageFromNotSubscribedTopic) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    ASSERT_THROW(consumer.fetchSingleTopic(topic1), std::runtime_error);
}

TEST(ConsumerTest, FetchMessageFromAllSubscribedTopics) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    Topic topic2{"topic2"};
    consumer.subscribe(topic1);
    consumer.subscribe(topic2);
    ASSERT_NO_THROW(consumer.fetchMessage());
}

TEST(ConsumerTest, FetchMessageWhenNoTopicSubscribed) {
    Consumer<IntMessage> consumer;
    ASSERT_NO_THROW(consumer.fetchMessage());
}
