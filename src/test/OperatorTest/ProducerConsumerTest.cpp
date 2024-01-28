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

using namespace messaging;

TEST(ProducerTest, SubscribeToNewTopic) {
    Producer<IntMessage> producer;
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    Topic topic1{"topic1"};
    manager->relate(topic1, queue);

    ASSERT_NO_THROW(producer.subscribe(topic1));
    manager->unrelate(topic1, queue);
}

TEST(ProducerTest, SubscribeToAlreadySubscribedTopic) {
    Producer<IntMessage> producer;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->relate(topic1, queue);
    producer.subscribe(topic1);
    ASSERT_THROW(producer.subscribe(topic1), std::runtime_error);
    manager->unrelate(topic1, queue);
}

TEST(ProducerTest, UnsubscribeFromSubscribedTopic) {
    Producer<IntMessage> producer;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->relate(topic1, queue);
    producer.subscribe(topic1);
    ASSERT_NO_THROW(producer.unsubscribe(topic1));
    manager->unrelate(topic1, queue);
}

TEST(ProducerTest, UnsubscribeFromNotSubscribedTopic) {
    Producer<IntMessage> producer;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->relate(topic1, queue);
    ASSERT_THROW(producer.unsubscribe(topic1), std::runtime_error);
    manager->unrelate(topic1, queue);
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
    manager->unrelate(topic1, queue);
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
    ASSERT_THROW(producer.broadcastMessage(message), std::runtime_error);
}

TEST(ProducerTest, BroadcastMessageWhenTopicsSubscribed) {
    Producer<IntMessage> producer;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    Topic topic1{"topic1"};
    Topic topic2{"topic2"};
    auto queue1 = Queue<IntMessage>();
    auto queue2 = Queue<IntMessage>();
    auto message = IntMessage(1);
    manager->relate(topic1, queue1);
    manager->relate(topic2, queue2);
    producer.subscribe(topic1);
    producer.subscribe(topic2);
    ASSERT_NO_THROW(producer.broadcastMessage(message));
}


TEST(ConsumerTest, SubscribeToNewTopic) {
    Consumer<IntMessage> consumer;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    manager->relate(topic1, queue);
    ASSERT_NO_THROW(consumer.subscribe(topic1));
}

TEST(ConsumerTest, SubscribeToAlreadySubscribedTopic) {
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    manager->relate(topic1, queue);
    consumer.subscribe(topic1);
    ASSERT_THROW(consumer.subscribe(topic1), std::runtime_error);
}

TEST(ConsumerTest, UnsubscribeFromSubscribedTopic) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    manager->relate(topic1, queue);
    consumer.subscribe(topic1);
    ASSERT_NO_THROW(consumer.unsubscribe(topic1));
}

TEST(ConsumerTest, UnsubscribeFromNotSubscribedTopic) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    manager->relate(topic1, queue);
    ASSERT_THROW(consumer.unsubscribe(topic1), std::runtime_error);
}

TEST(ConsumerTest, FetchMessageFromSubscribedEmptyTopic) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    manager->relate(topic1, queue);
    consumer.subscribe(topic1);
    ASSERT_THROW(consumer.fetchSingleTopic(topic1), std::runtime_error);
}

TEST(ConsumerTest, FetchMessageFromNotSubscribedTopic) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    manager->relate(topic1, queue);
    ASSERT_THROW(consumer.fetchSingleTopic(topic1), std::runtime_error);
}

TEST(ConsumerTest, FetchMessageFromAllSubscribedTopics) {
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue1;
    manager->relate(topic1, queue1);
    Topic topic2{"topic2"};
    Queue<IntMessage> queue2;
    manager->relate(topic2, queue2);
    Producer<IntMessage> producer;
    producer.subscribe(topic1);
    producer.subscribe(topic2);
    producer.broadcastMessage(IntMessage(1));
    consumer.subscribe(topic1);
    consumer.subscribe(topic2);
    ASSERT_NO_THROW(consumer.fetchMessage());
}

TEST(ConsumerTest, FetchMessageWhenNoTopicSubscribed) {
    Consumer<IntMessage> consumer;
    ASSERT_NO_THROW(consumer.fetchMessage());
}


TEST(ProducerTest, ConcurrentPublishToSubscribedTopic) {
    Producer<IntMessage> producer;
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    Topic topic1{"topic1"};
    manager->relate(topic1, queue);
    auto message = IntMessage(1);
    producer.subscribe(topic1);

    std::thread t1([&]() { producer.publishMessage(topic1, message); });
    std::thread t2([&]() { producer.publishMessage(topic1, message); });

    t1.join();
    t2.join();

    ASSERT_EQ(queue.size(), 2);
    manager->unrelate(topic1, queue);
}

TEST(ConsumerTest, ConcurrentFetchFromSubscribedTopic) {
    Consumer<IntMessage> consumer;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    manager->relate(topic1, queue);
    consumer.subscribe(topic1);

    Producer<IntMessage> producer;
    producer.subscribe(topic1);
    producer.publishMessage(topic1, IntMessage(1));
    producer.publishMessage(topic1, IntMessage(2));

    std::thread t1([&]() { consumer.fetchSingleTopic(topic1); });
    std::thread t2([&]() { consumer.fetchSingleTopic(topic1); });

    t1.join();
    t2.join();

    ASSERT_EQ(queue.size(), 0);
    manager->unrelate(topic1, queue);
}

TEST(ProducerConsumerTest, MultipleConsumersFetchingFromSingleProducer) {
    Producer<IntMessage> producer;
    Consumer<IntMessage> consumer1, consumer2, consumer3;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    manager->relate(topic1, queue);
    producer.subscribe(topic1);
    consumer1.subscribe(topic1);
    consumer2.subscribe(topic1);
    consumer3.subscribe(topic1);

    producer.publishMessage(topic1, IntMessage(1));
    producer.publishMessage(topic1, IntMessage(2));
    producer.publishMessage(topic1, IntMessage(3));

    std::thread t1([&]() { consumer1.fetchSingleTopic(topic1); });
    std::thread t2([&]() { consumer2.fetchSingleTopic(topic1); });
    std::thread t3([&]() { consumer3.fetchSingleTopic(topic1); });

    t1.join();
    t2.join();
    t3.join();

    ASSERT_EQ(queue.size(), 0);
    manager->unrelate(topic1, queue);
}

TEST(ProducerConsumerTest, NoDataMissedWithMultipleConsumersFetchingFromSingleProducer) {
    Producer<IntMessage> producer;
    Consumer<IntMessage> consumer1, consumer2, consumer3;
    Topic topic1{"topic1"};
    Queue<IntMessage> queue;
    auto manager = MessageQueueManager<IntMessage>::Instance();
    manager->flush();
    manager->relate(topic1, queue);
    producer.subscribe(topic1);
    consumer1.subscribe(topic1);
    consumer2.subscribe(topic1);
    consumer3.subscribe(topic1);

    producer.publishMessage(topic1, IntMessage(1));
    producer.publishMessage(topic1, IntMessage(2));
    producer.publishMessage(topic1, IntMessage(3));

    auto f1 = std::async(std::launch::async, [&]() { return consumer1.fetchSingleTopic(topic1); });
    auto f2 = std::async(std::launch::async, [&]() { return consumer1.fetchSingleTopic(topic1); });
    auto f3 = std::async(std::launch::async, [&]() { return consumer1.fetchSingleTopic(topic1); });

    ASSERT_EQ(f1.get()[0]->getContent() + f2.get()[0]->getContent() + f3.get()[0]->getContent(), 6);

    manager->unrelate(topic1, queue);
}
