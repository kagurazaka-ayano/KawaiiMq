/**
 * @file MessageQueueManager.h
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#ifndef KAWAIIMQ_MESSAGEQUEUEMANAGER_HPP
#define KAWAIIMQ_MESSAGEQUEUEMANAGER_HPP

#include "Topic.h"
#include <mutex>
#include <iostream>
#include <unordered_map>
#include "Queue.hpp"


namespace messaging {

    /**
     * a singleton for the given message type
     * @tparam T type of content this message contain
     */
    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    class MessageQueueManager {
        friend Queue<T>;
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
        auto getAllRelatedQueue(const Topic& topic);

    private:
        MessageQueueManager() = default;
        static std::once_flag flag;
        std::mutex mtx;
        static std::shared_ptr<MessageQueueManager> instance;
        std::unordered_map<Topic, std::vector<std::reference_wrapper<Queue<T>>>> topic_map;
        std::unordered_map<std::string, Topic> registered_topic;

    };

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::once_flag MessageQueueManager<T>::flag = std::once_flag();


    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::shared_ptr<MessageQueueManager<T>> MessageQueueManager<T>::instance = std::shared_ptr<MessageQueueManager<T>>();


    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::shared_ptr<MessageQueueManager<T>> MessageQueueManager<T>::Instance() {
        if (instance == nullptr) {
            std::call_once(flag, [&] { instance.reset(new MessageQueueManager<T>); });
        }
        return instance;
    }
    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void MessageQueueManager<T>::relate(const Topic &topic, Queue<T> &queue) {
        registered_topic.try_emplace(topic.getName(), topic);
        auto it = std::find_if(topic_map[topic].begin(), topic_map[topic].end(),
                               [&queue](const Queue<T> &q) { return &q == &queue; });
        if (it == topic_map[topic].end()) {
            topic_map[topic].push_back(std::ref(queue));
        } else {
            std::cerr << "Attempted to relate duplicated queue in same topic: " << topic.getName() << std::endl;
        }
    }


    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void MessageQueueManager<T>::unrelate(const Topic &topic, const Queue<T> &queue) {
        if (std::find(topic_map[topic].begin(), topic_map[topic].end(), queue) == topic_map[topic].end()) {
            std::remove(topic_map[topic].begin(), topic_map[topic].end(), queue);
            if (topic_map[topic].empty()) {
                registered_topic.erase(topic.getName());
            }
        }
        else {
            std::cerr << "Attempted to unrelate nonexistent queue from topic: " << topic.getName() << std::endl;
        }
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    auto MessageQueueManager<T>::getAllRelatedQueue(const Topic& topic) {
        return topic_map[topic];
    }
}
#endif //KAWAIIMQ_MESSAGEQUEUEMANAGER_HPP
