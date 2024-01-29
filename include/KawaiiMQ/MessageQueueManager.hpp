/**
 * @file MessageQueueManager.h
 * @author ayano
 * @date 1/22/24
 * @brief MessageQueueManager is a singleton that manages all queues and topics
*/

#ifndef KAWAIIMQ_MESSAGEQUEUEMANAGER_HPP
#define KAWAIIMQ_MESSAGEQUEUEMANAGER_HPP

#include "Topic.h"
#include <mutex>
#include <iostream>
#include <unordered_map>
#include "Queue.hpp"


namespace KawaiiMQ {

    /**
     * a singleton for the given message type
     * @tparam T type of content this message contain
     */
    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
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
         */
        void relate(const Topic& topic, Queue<T> &queue);

        /**
         * unrelate a queue from a topic
         * @param topic topic you want to unrelate
         * @param queue queue you want to unrelate
         */
        void unrelate(const Topic& topic, const Queue<T>& queue);

        /**
         * get all queues related to the topic
         * @param topic given topic
         * @return all queues related to the topic
         */
        auto getAllRelatedQueue(const Topic& topic) const;

        /**
         * get all topics that related to a queue
         * @return all topics
         */
        auto getRelatedTopic() const;

        /**
         * check if a queue is related to a topic
         * @param topic topic given
         * @param queue queue given
         * @return true if related, false otherwise
         */
        bool isRelated(const Topic& topic, const Queue<T>& queue);

        /**
         * check if a topic is related to any queue
         * @param topic topic given
         * @return true if related to any queue, false otherwise
         */
        bool isRelatedAny(const Topic& topic);

        /**
         * flush all queues and topics
         * @remark this function is for testing only
         * @warning this function will clear all queues and topics
         * @warning DO NOT USE THIS FUNCTION IN PRODUCTION
         */
        void flush();

    private:
        MessageQueueManager() = default;
        mutable std::shared_mutex mtx;
        std::unordered_map<Topic, std::vector<std::reference_wrapper<Queue<T>>>> topic_map;
        std::unordered_map<std::string, Topic> registered_topic;

    };


    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::shared_ptr<MessageQueueManager<T>> MessageQueueManager<T>::Instance() {
        static std::shared_ptr<MessageQueueManager> instance(new MessageQueueManager<T>());
        return instance;
    }
    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void MessageQueueManager<T>::relate(const Topic &topic, Queue<T> &queue) {
        std::lock_guard lock(mtx);
        auto& queues = topic_map[topic];
        if (std::find_if(queues.begin(), queues.end(), [&queue](const Queue<T>& q) {
            return &q == &queue;
        }) == queues.end()) {
            registered_topic.try_emplace(topic.getName(), topic);
            topic_map[topic].push_back(std::ref(queue));
        } else {
            throw std::runtime_error("Attempted to relate duplicated queue in same topic: " + topic.getName());
        }
    }


    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void MessageQueueManager<T>::unrelate(const Topic &topic, const Queue<T> &queue) {
        std::lock_guard lock(mtx);
        if (std::find_if(topic_map[topic].begin(), topic_map[topic].end(), [&queue](const Queue<T>& q) {
            return &q == &queue;
        }) != topic_map[topic].end()) {
            topic_map[topic].erase(std::remove_if(topic_map[topic].begin(), topic_map[topic].end(), [&queue](const Queue<T>& q) {
                return &q == &queue;
            }), topic_map[topic].end());
            if (topic_map[topic].empty()) {
                registered_topic.erase(topic.getName());
            }
        }
        else {
            throw std::runtime_error("Attempted to unrelate nonexistent queue from topic: " + topic.getName());
        }
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    auto MessageQueueManager<T>::getAllRelatedQueue(const Topic& topic) const {
        std::shared_lock lock(mtx);
        try {
            return topic_map.at(topic);
        }
        catch (std::out_of_range& e) {
            throw std::runtime_error("Topic " + topic.getName() + " didn't relate to any queue");
        }

    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    bool MessageQueueManager<T>::isRelated(const Topic& topic, const Queue<T>& queue) {
        std::shared_lock lock(mtx);
        auto& queues = topic_map[topic];
        return std::find_if(queues.begin(), queues.end(), [&queue](const Queue<T>& q){return &q == &queue;}) != queues.end();
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    auto MessageQueueManager<T>::getRelatedTopic() const {
        std::shared_lock lock(mtx);
        std::vector<Topic> ret;
        for(const auto& i : topic_map) {
            ret.push_back(i.first);
        }
        return ret;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    bool MessageQueueManager<T>::isRelatedAny(const Topic& topic){
        std::shared_lock lock(mtx);
        return topic_map.find(topic) != topic_map.end();
    }
    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void MessageQueueManager<T>::flush() {
        std::lock_guard lock(mtx);
        topic_map.clear();
        registered_topic.clear();
    }
}
#endif //KAWAIIMQ_MESSAGEQUEUEMANAGER_HPP
