/**
 * @file Consumer.hpp
 * @author ayano
 * @date 1/25/24
 * @brief A consumer of the message queue
*/

#ifndef KAWAIIMQ_CONSUMER_HPP
#define KAWAIIMQ_CONSUMER_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include "Topic.h"
#include "IMessage.hpp"
#include "utility.hpp"
#include "MessageQueueManager.hpp"

namespace messaging {
    /**
     * The client of the message queue
     * @remark A client is a user of the message queue system. It can subscribe to multiple topics and fetch messages from topics.
     */
    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    class Consumer {
    public:
        Consumer() = default;

        explicit Consumer(std::vector<Topic> topics);

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
         * fetch all messages from subscribed topics
         * @return all messages from subscribed topics
         */
        std::unordered_map<Topic, std::vector<std::shared_ptr<T>>> fetchMessage();



        /**
         * fetch all messages from a single topic
         * @param topic given topic
         * @return all messages from the topic
         */
        std::vector<std::shared_ptr<T>> fetchSingleTopic(const Topic& topic);

    private:
        std::mutex mtx;
        std::vector<Topic> subscribed;
    };


    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::unordered_map<Topic, std::vector<std::shared_ptr<T>>> Consumer<T>::fetchMessage() {
        std::lock_guard<std::mutex> lock(mtx);
        auto manager = MessageQueueManager<T>::Instance();
        std::unordered_map<Topic, std::vector<std::shared_ptr<T>>> ret;
        for(const auto& i : subscribed) {
            auto queue = manager->getAllRelatedQueue(i);
            for(auto& j : queue) {
                auto message = j.get().wait();
                {
                    ret[i].push_back(message);
                }
            }
        }
        return ret;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::vector<std::shared_ptr<T>> Consumer<T>::fetchSingleTopic(const Topic &topic) {
        if(std::find(subscribed.begin(), subscribed.end(), topic) == subscribed.end()) {
            throw std::runtime_error("topic not subscribed");
        }
        std::lock_guard<std::mutex> lock(mtx);
        auto manager = MessageQueueManager<T>::Instance();
        std::vector<std::shared_ptr<T>> ret;
        auto queue = manager->getAllRelatedQueue(topic);
        for (auto &j : queue) {
            if (j.get().empty()) {
                throw std::runtime_error("queue empty");
            }
            auto message = j.get().wait();
            ret.push_back(message);
        }
        return ret;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    messaging::Consumer<T>::Consumer(std::vector <Topic> topics) : subscribed(std::move(topics)) {

    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void Consumer<T>::subscribe(const Topic &topic) {
        if (std::find(subscribed.begin(), subscribed.end(), topic) == subscribed.end()) {
            subscribed.push_back(topic);
        }
        else {
            throw std::runtime_error("Attempting to subscribe a subscribed topic! Topic: " + topic.getName());
        }
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void Consumer<T>::unsubscribe(const Topic &topic) {
        auto it = std::find(subscribed.begin(), subscribed.end(), topic);
        if (it != subscribed.end()) {
            subscribed.erase(it);
        }
        else {
            throw std::runtime_error("Attempting to unsubscribe a non-subscribed topic! Topic: " + topic.getName());
        }
    }

} // messaging

#endif //KAWAIIMQ_CONSUMER_HPP
