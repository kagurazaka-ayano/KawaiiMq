# Examples

> All examples assume you included the header file `KawaiiMQ/kawaiiMQ.h`

## 1. Creating a message queue

```c++
KawaiiMQ::Queue queue;
```

## 2. Relating a message queue with a topic

```c++

KawaiiMQ::MessageQueue queue;
KawaiiMQ::Topic topic("topic1");
KawaiiMQ::MessageQueueManager::Instance()->relate(topic, queue);

```

## 3. Publishing a message

```c++

KawaiiMQ::Queue queue;
KawaiiMQ::Topic topic("topic1");
KawaiiMQ::MessageQueueManager::Instance()->relate(topic, queue);
KawaiiMQ::Producer producer;
producer.subscribe(topic);
auto message = KawaiiMQ::makeMessage(0);
producer.publishMessage(topic, message);

```

## 4. Consuming a message

```c++
KawaiiMQ::Queue queue;
KawaiiMQ::Topic topic("topic1");
KawaiiMQ::MessageQueueManager::Instance()->relate(topic, queue);
KawaiiMQ::Producer producer;
producer.subscribe(topic);
auto message = KawaiiMQ::makeMessage(0);
producer.publishMessage(topic, message);
auto consumer = KawaiiMQ::Consumer({topic});
auto messages = consumer.fetchMessage();
std::cout << getMessage<int>(messages[topic][0]) << std::endl;
```

The output will be `0`.

## 5. Unrelate a message queue with a topic

```c++
KawaiiMQ::Queue queue;
KawaiiMQ::Topic topic("topic1");
KawaiiMQ::MessageQueueManager::Instance()->relate(topic, queue);
KawaiiMQ::MessageQueueManager::Instance()->unrelate(topic, queue);
```

> Note this method will wait till the queue is empty before unrelating the queue with the topic.
> 
## 6. Unsubscribe a topic, both consumer and producer

```c++
KawaiiMQ::Queue queue;
KawaiiMQ::Topic topic("topic1");
KawaiiMQ::MessageQueueManager::Instance()->relate(topic, queue);
KawaiiMQ::Producer producer;
auto consumer = KawaiiMQ::Consumer({topic});
producer.subscribe(topic);
consumer.unsubscribe(topic);
producer.unsubscribe(topic);
```
