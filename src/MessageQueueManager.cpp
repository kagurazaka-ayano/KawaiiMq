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

    void MessageQueueManager::relate(const Topic &topic, Queue &queue) {
        std::lock_guard lock(mtx);
        auto& queues = topic_map[topic];
        if (std::find_if(queues.begin(), queues.end(), [&queue](const Queue& q) {
            return &q == &queue;
        }) == queues.end()) {
            related_topic.try_emplace(topic.getName(), topic);
            topic_map[topic].push_back(std::ref(queue));
        } else {
            throw QueueException("Attempted to relate duplicated queue in same topic: " + topic.getName());
        }
    }


    void MessageQueueManager::unrelate(const Topic &topic, Queue &queue) {
        std::unique_lock lock(mtx);
        queue.getSafeCond().wait(lock, [&queue](){return queue.empty();});
        if (std::find_if(topic_map[topic].begin(), topic_map[topic].end(), [&queue](const Queue& q) {
            return &q == &queue;
        }) != topic_map[topic].end()) {
            topic_map[topic].erase(std::remove_if(topic_map[topic].begin(), topic_map[topic].end(), [&queue](const Queue& q) {
                return &q == &queue;
            }), topic_map[topic].end());
            if (topic_map[topic].empty()) {
                related_topic.erase(topic.getName());
            }
        }
        else {
            throw QueueException("Attempted to unrelate nonexistent queue from topic: " + topic.getName());
        }
    }

    std::vector<std::reference_wrapper<Queue>> MessageQueueManager::getAllRelatedQueue(const Topic& topic) const {
        std::shared_lock lock(mtx);
        try {
            return topic_map.at(topic);
        }
        catch (std::out_of_range& e) {
            throw TopicException("Topic " + topic.getName() + " didn't relate to any queue");
        }

    }

    bool MessageQueueManager::isRelated(const Topic& topic, const Queue& queue) {
        std::shared_lock lock(mtx);
        auto& queues = topic_map[topic];
        return std::find_if(queues.begin(), queues.end(), [&queue](const Queue& q){return &q == &queue;}) != queues.end();
    }

    std::vector<Topic> MessageQueueManager::getRelatedTopic() const {
        std::shared_lock lock(mtx);
        std::vector<Topic> ret;
        ret.reserve(topic_map.size());
        for(const auto& i : topic_map) {
            ret.push_back(i.first);
        }
        return ret;
    }

    bool MessageQueueManager::isRelatedAny(const Topic& topic){
        std::shared_lock lock(mtx);
        return topic_map.find(topic) != topic_map.end();
    }

#ifdef KAWAIIMQ_TEST

    void MessageQueueManager::flush() {
        std::lock_guard lock(mtx);
        topic_map.clear();
        related_topic.clear();
    }
#endif
}
