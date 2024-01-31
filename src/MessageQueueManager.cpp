/**
 * @file MessageQueueManager.cpp
 * @author ayano
 * @date 1/29/24
 * @brief
*/

#include "MessageQueueManager.h"

namespace KawaiiMQ {
    std::shared_ptr<MessageQueueManager> MessageQueueManager::Instance() {
        static std::shared_ptr<MessageQueueManager> instance(new MessageQueueManager());
        return instance;
    }

    void MessageQueueManager::relate(const Topic &topic, std::shared_ptr<Queue> queue) {
        std::lock_guard lock(mtx);
        bool find = false;
        for (const auto& q : topic_map[topic]) {
            if (q->getName() == queue->getName()) {
                find = true;
                break;
            }
        }
        if (!find) {
            topic_map[topic].push_back(queue);
        } else {
            throw QueueException("Attempted to relate duplicated queue in same topic: " + topic.getName());
        }
    }


    void MessageQueueManager::unrelate(const Topic &topic, std::shared_ptr<Queue> queue) {
        std::unique_lock lock(mtx);
        queue->getSafeCond().wait_for(lock, std::chrono::milliseconds(queue->getSafeTimeout()), [&queue](){return !queue->empty();});
        bool find = false;
        for (const auto& q : topic_map[topic]) {
            if (q->getName() == queue->getName()) {
                find = true;
                break;
            }
        }
        if (find) {
            auto &queues = topic_map[topic];
            auto it = std::find_if(
                    queues.begin(), queues.end(),
                    [&queue](const std::shared_ptr<Queue> &q) {
                        return queue->getName() == q->getName();
                    });
            queues.erase(it);
        }
        else{
            throw QueueException("Attempted to unrelate nonexistent queue from topic: " + topic.getName());
        }
    }

    std::vector<std::shared_ptr<Queue>> MessageQueueManager::getAllRelatedQueue(const Topic& topic) const {
        std::shared_lock lock(mtx);
        try {
            return topic_map.at(topic);
        }
        catch (std::out_of_range& e) {
            throw TopicException("Topic " + topic.getName() + " is not related to any queue");
        }

    }

    bool MessageQueueManager::isRelated(const Topic& topic, std::shared_ptr<Queue> queue) {
        std::shared_lock lock(mtx);
        auto& queues = topic_map[topic];
        return std::find_if(queues.begin(), queues.end(), [&queue](const std::shared_ptr<Queue>& q) {
            return q->getName() == queue->getName();
        }) != queues.end();
    }

    std::vector<Topic> MessageQueueManager::getRelatedTopic() const {
        std::shared_lock lock(mtx);
        std::vector<Topic> ret;
        ret.reserve(related_topic.size());
        for(const auto& i : related_topic) {
            ret.push_back(i.second);
        }
        return ret;
    }

    bool MessageQueueManager::isRelatedAny(const Topic& topic){
        std::shared_lock lock(mtx);
        return topic_map.find(topic) != topic_map.end();
    }

#ifdef TEST
    void MessageQueueManager::flush() {
        std::lock_guard lock(mtx);
        topic_map.clear();
        related_topic.clear();
    }
#endif
}
