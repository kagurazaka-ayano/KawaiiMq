# Examples

> All examples assume you included the header file `KawaiiMQ/kawaiiMQ.h`


## Usage example

To use a message queue, you need to create a queue and a topic.

```cpp
KawaiiMQ::Queue queue;
KawaiiMQ::Topic topic("topic1");
```

Then, you need to relate the queue with the topic.

```cpp
KawaiiMQ::MessageQueueManager::Instance()->relate(topic, queue);
```

After that, you can create a producer and subscribe to the topic.

```cpp
KawaiiMQ::Producer producer;
producer.subscribe(topic);
```

To make a message, use the `makeMessage` function.

```cpp
auto message = KawaiiMQ::makeMessage(0);
```

Then, you can publish the message.

```cpp
producer.publishMessage(topic, message);
```

To consume the message, you need to create a consumer and fetch the message.

```cpp
auto consumer = KawaiiMQ::Consumer({topic});
auto messages = consumer.fetchMessage();
```

The return value is a map of topics and messages. You can use the `getMessage` function to get the message.

```cpp
std::cout << getMessage<int>(messages[topic][0]) << std::endl;
```

You can of course only publish message on a single topic or consume messages from a single topic.

```cpp
KawaiiMQ::Queue queue;
KawaiiMQ::Topic topic("topic1");
KawaiiMQ::MessageQueueManager::Instance()->relate(topic, queue);
KawaiiMQ::Producer producer;
producer.subscribe(topic);
auto message = KawaiiMQ::makeMessage(0);
producer.publishMessage(topic, message);
auto consumer = KawaiiMQ::Consumer({topic});
consumer.subscribe(topic);
auto messages = consumer.fetchSingleTopic(topic);
std::cout << getMessage<int>(messages[0]) << std::endl;
```

## More examples

### Creating a message queue

```cpp
KawaiiMQ::Queue queue;
```

### Relating a message queue with a topic

```cpp

KawaiiMQ::MessageQueue queue;
KawaiiMQ::Topic topic("topic1");
KawaiiMQ::MessageQueueManager::Instance()->relate(topic, queue);

```

### Publishing a message

```cpp
KawaiiMQ::Queue queue;
KawaiiMQ::Topic topic("topic1");
KawaiiMQ::MessageQueueManager::Instance()->relate(topic, queue);
KawaiiMQ::Producer producer;
producer.subscribe(topic);
auto message = KawaiiMQ::makeMessage(0);
producer.publishMessage(topic, message);
```

### Consuming a message

```cpp
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

### Unrelate a message queue with a topic

```cpp
KawaiiMQ::Queue queue;
KawaiiMQ::Topic topic("topic1");
KawaiiMQ::MessageQueueManager::Instance()->relate(topic, queue);
KawaiiMQ::MessageQueueManager::Instance()->unrelate(topic, queue);
```

> Note this method will wait till the queue is empty before unrelating the queue with the topic.

### Unsubscribe a topic, both consumer and producer

```cpp
KawaiiMQ::Queue queue;
KawaiiMQ::Topic topic("topic1");
KawaiiMQ::MessageQueueManager::Instance()->relate(topic, queue);
KawaiiMQ::Producer producer;
auto consumer = KawaiiMQ::Consumer({topic});
producer.subscribe(topic);
consumer.unsubscribe(topic);
producer.unsubscribe(topic);
```
