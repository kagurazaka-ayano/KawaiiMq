/**
 * @file MessageQueueManager.h
 * @author ayano
 * @date 1/22/24
 * @brief MessageQueueManager is a singleton that manages all queues and topics
*/

#ifndef KAWAIIMQ_MESSAGEQUEUEMANAGER_H
#define KAWAIIMQ_MESSAGEQUEUEMANAGER_H

#include "Topic.h"
#include <mutex>
#include <iostream>
#include <unordered_map>
#include "Queue.h"


namespace KawaiiMQ {

    /**
     * a singleton for managing the message queue
     */
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
         * relate a queue to a topic
         * @param topic topic you want to relate
         * @param queue queue you want to relate
         * @exception TopicException if the topic is already related to the queue
         */
        void relate(const Topic& topic, std::shared_ptr<Queue> queue);

        /**
         * unrelate a queue from a topic
         * @param topic topic you want to unrelate
         * @param queue queue you want to unrelate
         * @exception TopicException if the topic is not related to the queue
         * @remark this function will wait until the queue is empty
         */
        void unrelate(const Topic& topic, std::shared_ptr<Queue> queue);

        /**
         * get all queues related to the topic
         * @param topic given topic
         * @return all queues related to the topic
         * @exception TopicException if the topic is not related to any queue
         */
        std::vector<std::shared_ptr<Queue>> getAllRelatedQueue(const Topic& topic) const;

        /**
         * get all topics that related to a queue
         * @return all topics
         */
        std::vector<Topic> getRelatedTopic() const;

        /**
         * check if a queue is related to a topic
         * @param topic topic given
         * @param queue queue given
         * @return true if related, false otherwise
         */
        bool isRelated(const Topic& topic, std::shared_ptr<Queue> queue);

        /**
         * check if a topic is related to any queue
         * @param topic topic given
         * @return true if related to any queue, false otherwise
         */
        bool isRelatedAny(const Topic& topic);

#ifdef TEST
        /**
         * flush all queues and topics
         * @remark this function is for testing only so I marked it
         * @warning this function will clear all queues and topics
         * @warning DO NOT USE THIS FUNCTION IN PRODUCTION
         */
        void flush();
#endif
    private:
        MessageQueueManager() = default;
        mutable std::shared_mutex mtx;
        std::unordered_map<Topic, std::vector<std::shared_ptr<Queue>>> topic_map;
        std::unordered_map<std::string, Topic> related_topic;
    };
}
#endif //KAWAIIMQ_MESSAGEQUEUEMANAGER_H
