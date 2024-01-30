/**
 * @file Producer.hpp
 * @author ayano
 * @date 1/27/24
 * @brief A producer of the message queue
*/

#ifndef KAWAIIMQ_PRODUCER_HPP
#define KAWAIIMQ_PRODUCER_HPP

#include "Topic.h"
#include "Message.h"
#include "MessageQueueManager.hpp"

namespace KawaiiMQ {
    /**
     * Producer is a class that can publish message to a topic
     * @tparam T type of message this consumer accepts
     */
    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    class Producer {
    public:

        /**
         * subscribe a topic
         * @param topic topic you want to subscribe
         */
        void subscribe(const Topic& topic) {
            std::lock_guard lock(mtx);
            auto manager = MessageQueueManager<T>::Instance();
            if(!manager->isRelatedAny(topic)) {
                throw std::runtime_error("topic not related to any queue");
            }
            if (std::find(subscribed.begin(), subscribed.end(), topic) != subscribed.end()) {
                throw std::runtime_error("topic already subscribed");
            }
            subscribed.push_back(topic);
        }

        /**
         * unsubscribe a topic
         * @param topic
         */
        void unsubscribe(const Topic& topic) {
            std::lock_guard lock(mtx);
            if (std::find(subscribed.begin(), subscribed.end(),topic) == subscribed.end()) {
                throw std::runtime_error("topic not subscribed");
            }
            std::remove(subscribed.begin(), subscribed.end(), topic);
        }

        /**
         * publish a message to a topic
         * @param topic topic you want to publish
         * @param message message you want to publish
         * @remark make sure the topic given is related to any queue
         */
        void publishMessage(const Topic& topic, T message) {
            if (std::find(subscribed.begin(), subscribed.end(), topic) == subscribed.end()) {
                throw std::runtime_error("topic not subscribed");
            }
            auto manager = MessageQueueManager<T>::Instance();
            auto queues = manager->getAllRelatedQueue(topic);
            for(auto& queue : queues) {
                queue.get().push(std::move(message));
            }
        }

        /**
         * broadcast a message to all topics
         * @param message message you want to broadcast
         * @remark make sure all the topics are related to any queue
         */
        void broadcastMessage(T message) {
            auto manager = MessageQueueManager<T>::Instance();
            if (subscribed.empty()) {
                throw std::runtime_error("no topic subscribed");
            }
            for (const auto& topic: subscribed) {
                auto queues = manager->getAllRelatedQueue(topic);
                for(auto& queue : queues) {
                    queue.get().push(std::move(message));
                }
            }
        }
    private:
        std::vector<Topic> subscribed;
        std::mutex mtx;
    };
}

#endif //KAWAIIMQ_PRODUCER_HPP
