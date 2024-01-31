/**
 * @file Consumer.hpp
 * @author ayano
 * @date 1/25/24
 * @brief A consumer of the message queue
*/

#ifndef KAWAIIMQ_CONSUMER_H
#define KAWAIIMQ_CONSUMER_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "Topic.h"
#include "Message.h"
#include "utility.hpp"
#include "MessageQueueManager.h"

namespace KawaiiMQ {
    /**
     * The client of the message queue
     * @remark A client is a user of the message queue system. It can subscribe to multiple topics and fetch messages from topics.
     */
    class Consumer {
    public:
        Consumer() = default;

        explicit Consumer(std::vector<Topic> topics);

        explicit Consumer(const std::string& name);

        /**
         * subscribe a topic
         * @param topic given topic
         */
        void subscribe(const Topic &topic);

        /**
         * unsubscribe a topic
         * @param topic given topic
         */
        void unsubscribe(const Topic& topic);

        /**
         * fetch all messages from subscribed topics
         * @return all messages from subscribed topics
         */
        std::unordered_map<Topic, std::vector<std::shared_ptr<MessageData>>> fetchMessage();

        /**
         * fetch all messages from a single topic
         * @param topic given topic
         * @return all messages from the topic
         */
        std::vector<std::shared_ptr<MessageData>> fetchSingleTopic(const Topic& topic);

        std::string getName() const;

        std::vector<Topic> getSubscribedTopics() const;

    private:
        std::mutex mtx;
        std::string name;
        std::vector<Topic> subscribed;
    };

} // KawaiiMQ

#endif //KAWAIIMQ_CONSUMER_H
