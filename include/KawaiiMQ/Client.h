/**
 * @file Client.h
 * @author ayano
 * @date 1/25/24
 * @brief The Client class
*/

#ifndef KAWAIIMQ_CLIENT_H
#define KAWAIIMQ_CLIENT_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "Topic.h"
#include "IMessage.h"
#include "utility.hpp"
#include "MessageQueueManager.hpp"

namespace messaging {
    /**
     * The client of the message queue
     * @remark A client is a user of the message queue system. It can subscribe to multiple topics and fetch messages from topics.
     */
    class Client {
    public:
        Client() = default;

        explicit Client(std::vector<Topic> topics);

        /**
         * subscribe a topic
         * @param topic given topic
         */
        void subscribe(const Topic& topic);

        /**
         * unsubscribe a topic
         * @param topic given topic
         */
        void unsubscribe(const Topic& topic);

        /**
         * fetch message from all subscribed topics
         * @tparam T type of message
         * @return a map, key is topic, value is a vector of messages from each subscribed topic
         */
        template<typename T>
        requires DerivedFromTemplate<IMessage, T>
        std::unordered_map<Topic, std::vector<std::shared_ptr<T>>> fetchMessage();

        /**
         * publish message to all subscribed topics
         * @tparam T type of message
         * @param message message you want to publish
         */
        template<typename T>
        requires DerivedFromTemplate<IMessage, T>
        void publishMessage(const T& message);

        /**
         * fetch message from a single topic
         * @tparam T type of message
         * @param topic topic you want to fetch from
         * @return a vector containing all messages from the topic
         */
        template<typename T>
        requires DerivedFromTemplate<IMessage, T>
        std::vector<std::shared_ptr<T>> fetchSingleTopic(const Topic& topic);

        /**
         * publish message to a single topic
         * @tparam T type of message
         * @param topic topic you want to publish to
         * @param message message you want to publish
         */
        template<typename T>
        requires DerivedFromTemplate<IMessage, T>
        void publishSingleMessage(const Topic& topic, const T& message);

    private:
        std::vector<Topic> subscribed;
    };


    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::unordered_map<Topic, std::vector<std::shared_ptr<T>>> Client::fetchMessage() {
        auto manager = MessageQueueManager<T>::Instance();
        std::unordered_map<Topic, std::vector<std::shared_ptr<T>>> ret;
        for(const auto& i : subscribed) {
            auto queue = manager->getAllRelatedQueue(i);
            for(auto& j : queue) {
                auto message = j.get().wait();
                ret[i].push_back(message);
            }
        }
        return ret;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void Client::publishMessage(const T& message) {
        auto manager = MessageQueueManager<T>::Instance();
        for (const auto& i: subscribed) {
            auto queue = manager->getAllRelatedQueue(i);
            for (auto& j : queue) {
                j.get().push(message);
            }
        }
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::vector<std::shared_ptr<T>> Client::fetchSingleTopic(const Topic &topic) {

        auto manager = MessageQueueManager<T>::Instance();
        std::vector<std::shared_ptr<T>> ret;
        auto queue = manager->getAllRelatedQueue(topic);
        for (auto &j : queue) {
            auto message = j.get().wait();
            ret.push_back(message);
        }
        return ret;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void Client::publishSingleMessage(const Topic &topic, const T &message) {
        auto manager = MessageQueueManager<T>::Instance();
        auto queue = manager->getAllRelatedQueue(topic);
        for (auto &i : queue) {
            i.get().push(message);
        }
    }

} // messaging

#endif //KAWAIIMQ_CLIENT_H
