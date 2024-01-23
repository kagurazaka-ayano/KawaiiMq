/**
 * @file MessageQueueManager.h
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#ifndef MESSAGEQUEUE_MESSAGEQUEUEMANAGER_H
#define MESSAGEQUEUE_MESSAGEQUEUEMANAGER_H

#include "Queue.hpp"
#include "Topic.h"
#include <mutex>
#include <iostream>
#include <unordered_map>
namespace messaging {

    /**
     * a singleton for the given message type
     * @tparam T type of content this message contain
     */
    template<typename T>
    class MessageQueueManager {
    public:
        MessageQueueManager &operator=(const MessageQueueManager &other) = delete;

        MessageQueueManager(const MessageQueueManager &other) = delete;

        MessageQueueManager(MessageQueueManager &&other) = delete;

        /**
         * get the instance
         * @return instance of this class
         */
        static std::shared_ptr<MessageQueueManager> Instance();

        /**
         * subscribe a queue to a topic
         * @param topic topic you want to subscribe
         * @param queue queue you want to subscribe
         */
        void subscribe(const Topic& topic, const Queue<T>& queue);

        /**
         * unsubscribe a queue from a topic
         * @param topic topic you want to unsubscribe
         * @param queue queue you want to unsubscribe
         */
        void unsubscribe(const Topic& topic, const Queue<T>& queue);

        /**
         * publish a message to the given topic
         * @param message message going to be published
         * @param topic topic the message will go to
         */
        void publish(const IMessage<T>& message, const Topic& topic);

        /**
         * retrieve a message from given queue
         * @param queue given queue
         * @return retrieved message
         */
        [[nodiscard]] std::shared_ptr<IMessage<T>> retrieve(Queue<T> queue) const;

    private:
        MessageQueueManager() = default;

        static std::once_flag flag;
        static std::shared_ptr<MessageQueueManager> instance;
        std::unordered_map<Topic, std::vector<Queue<T>>> topic_map;
        std::unordered_map<std::string, Topic> registered_topic;

    };
}
#endif //MESSAGEQUEUE_MESSAGEQUEUEMANAGER_H
