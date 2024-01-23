/**
 * @file MessageQueueManager.h
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#ifndef MESSAGEQUEUE_MESSAGEQUEUEMANAGER_HPP
#define MESSAGEQUEUE_MESSAGEQUEUEMANAGER_HPP

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
         * subscribe a queue to a topic
         * @param topic topic you want to subscribe
         * @param queue queue you want to subscribe
         */
        void subscribe(const Topic& topic, Queue<T> &queue);

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
        void publish(const T& message, const Topic& topic);

        /**
         * retrieve a message from given queue
         * @param queue given queue
         * @return retrieved message
         */
        [[nodiscard]] std::shared_ptr<T> retrieve(Queue<T> &queue);

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
    void MessageQueueManager<T>::subscribe(const Topic &topic, Queue<T> &queue) {
        registered_topic.try_emplace(topic.getName(), topic);
        auto it = std::find_if(topic_map[topic].begin(), topic_map[topic].end(),
                               [&queue](const Queue<T> &q) { return &q == &queue; });
        if (it == topic_map[topic].end()) {
            topic_map[topic].push_back(std::ref(queue));
        } else {
            std::cerr << "Attempted to subscribe duplicated queue in same topic: " << topic.getName() << std::endl;
        }
    }


    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void MessageQueueManager<T>::unsubscribe(const Topic &topic, const Queue<T> &queue) {
        if (std::find(topic_map[topic].begin(), topic_map[topic].end(), queue) == topic_map[topic].end()) {
            std::remove(topic_map[topic].begin(), topic_map[topic].end(), queue);
            if (topic_map[topic].empty()) {
                registered_topic.erase(topic.getName());
            }
        }
        else {
            std::cerr << "Attempted to unsubscribe nonexistent queue from topic: " << topic.getName() << std::endl;
        }
    }
    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void MessageQueueManager<T>::publish(const T &message, const Topic &topic) {
        auto queue = topic_map[topic];
        for (int i = 0; i < queue.size(); ++i) {
            queue[i].get().push(message);
        }
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::shared_ptr<T> MessageQueueManager<T>::retrieve(Queue<T> &queue) {
        std::lock_guard lock(mtx);
        auto ret = queue.wait();
        return ret;
    }
}
#endif //MESSAGEQUEUE_MESSAGEQUEUEMANAGER_HPP
