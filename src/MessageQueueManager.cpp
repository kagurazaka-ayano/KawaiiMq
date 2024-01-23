/**
 * @file MessageQueueManager.cpp
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#include "MessageQueueManager.h"
namespace messaging {
    template<typename T>
    std::shared_ptr<MessageQueueManager<T>> MessageQueueManager<T>::Instance() {
        if (instance == nullptr) {
            std::call_once(flag, [&] { instance.reset(new MessageQueueManager<T>); });
        }
        return instance;
    }
    template<typename T>
    void MessageQueueManager<T>::subscribe(const Topic &topic, const Queue<T> &queue) {
        registered_topic.try_emplace(topic.getName(), topic);
        if (std::find(topic_map[topic].begin(), topic_map[topic].end(),queue) != topic_map[topic].end()) {
            topic_map[topic].push_back(queue);
        }
        else {
            std::cerr << "Attempted to add duplicated queue in same topic: " << topic.getName() << std::endl;
        }
    }


    template<typename T>
    void MessageQueueManager<T>::unsubscribe(const Topic &topic, const Queue<T> &queue) {
        if (std::find(topic_map[topic].begin(), topic_map[topic].end(), queue) != topic_map[topic].end()) {
            std::remove(topic_map[topic].begin(), topic_map[topic].end(), queue);
            if (topic_map[topic].empty()) {
                registered_topic.erase(topic.getName());
            }
        }
        else {
            std::cerr << "Attempted to remove nonexistent queue from topic: " << topic.getName() << std::endl;
        }
    }
    template<typename T>
    void MessageQueueManager<T>::publish(const IMessage<T> &message, const Topic &topic) {

    }

    template<typename T>
    std::shared_ptr<IMessage<T>> MessageQueueManager<T>::retrieve(Queue<T> queue) const {
        return queue.wait();
    }
}
