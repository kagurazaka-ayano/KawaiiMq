/**
 * @file Producer.cpp
 * @author ayano
 * @date 1/30/24
 * @brief
*/

#include "Producer.h"

namespace KawaiiMQ {

    /**
     * subscribe a topic
     * @param topic topic you want to subscribe
     */
    void Producer::subscribe(const Topic& topic) {
        std::lock_guard lock(mtx);
        auto manager = MessageQueueManager::Instance();
        if(!manager->isRelatedAny(topic)) {
            throw TopicException("topic not related to any queue");
        }
        if (std::find(subscribed.begin(), subscribed.end(), topic) != subscribed.end()) {
            throw TopicException("topic already subscribed");
        }
        subscribed.push_back(topic);
    }

    /**
     * unsubscribe a topic
     * @param topic
     */
    void Producer::unsubscribe(const Topic& topic) {
        std::lock_guard lock(mtx);
        if (std::find(subscribed.begin(), subscribed.end(),topic) == subscribed.end()) {
            throw TopicException("topic not subscribed");
        }
        std::remove(subscribed.begin(), subscribed.end(), topic);
    }
}
