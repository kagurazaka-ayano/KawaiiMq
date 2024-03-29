/**
 * @file Producer.hpp
 * @author ayano
 * @date 1/27/24
 * @brief A producer of the message queue
*/

#ifndef KAWAIIMQ_PRODUCER_H
#define KAWAIIMQ_PRODUCER_H

#include "Topic.h"
#include "Message.h"
#include "MessageQueueManager.h"

namespace KawaiiMQ {

    class Producer {
    public:
        explicit Producer(const std::string &name);

        explicit Producer(std::vector<Topic> topics, const std::string &name);

        /**
         * subscribe to a topic
         * @param topic topic you want to subscribe
         * @exception TopicException Will throw if the topic is not related to any queue
         * @exception TopicException Will throw if the topic is already subscribed
         */
        void subscribe(const Topic& topic);

        /**
         * unsubscribe a topic
         * @param topic
         * @exception TopicException Will throw if the topic is not subscribed
         */
        void unsubscribe(const Topic& topic);

        /**
         * publish a message to a topic
         * @param topic topic you want to publish
         * @param message message you want to publish
         * @exception TopicException Will throw if the topic is not subscribed
         */
        template<typename T>
        void publishMessage(const Topic& topic, std::shared_ptr<T> message) {
            if (std::find(subscribed.begin(), subscribed.end(), topic) == subscribed.end()) {
                throw TopicException("topic not subscribed");
            }
            auto manager = MessageQueueManager::Instance();
            auto queues = manager->getAllRelatedQueue(topic);
            for(auto& queue : queues) {
                queue->push(std::move(message));
            }
        }

        /**
         * broadcast a message to all topics
         * @param message message you want to broadcast
         * @exception TopicException Will throw if no topic is subscribed
         */
        template<typename T>
        void broadcastMessage(std::shared_ptr<T> message) {
            auto manager = MessageQueueManager::Instance();
            if (subscribed.empty()) {
                throw TopicException("no topic subscribed");
            }
            for (const auto& topic: subscribed) {
                auto queues = manager->getAllRelatedQueue(topic);
                for(auto& queue : queues) {
                    queue->push(std::move(message));
                }
            }
        }

        /**
         * get the name of the producer
         * @return name of the producer
         */
        std::string getName() const;

        /**
         * get all subscribed topics
         * @return all subscribed topic, in a vector
         */
        std::vector<Topic> getSubscribedTopics() const;
    private:
        std::vector<Topic> subscribed;
        std::mutex mtx;
        std::string name;
    };
}

#endif //KAWAIIMQ_PRODUCER_H
