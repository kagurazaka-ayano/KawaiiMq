/**
 * @file MessageQueueManager.cpp
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#include "MessageQueueManager.h"
namespace messaging {
    std::shared_ptr<MessageQueueManager> MessageQueueManager::Instance() {
        if (instance == nullptr) {
            std::call_once(flag, [&] { instance.reset(new MessageQueueManager); });
        }
        return instance;
    }

    void MessageQueueManager::subscribe(const Topic &topic, const Queue &queue) {
        registered_topic.try_emplace(topic.getName(), topic);
        if (std::find(topic_map[topic].begin(), topic_map[topic].end(),queue) != topic_map[topic].end()) {
            topic_map[topic].push_back(queue);
        }
        else {
            std::cerr << "Attempted to add duplicated queue in same topic: " << topic.getName() << std::endl;
        }
    }

    void MessageQueueManager::unsubscribe(const Topic &topic, const Queue &queue) {
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

    void MessageQueueManager::publish(const IMessage &message, const Topic &topic) {

    }

    std::shared_ptr<IMessage> MessageQueueManager::retrieve(Queue queue) const {
        return queue.wait();
    }
}
