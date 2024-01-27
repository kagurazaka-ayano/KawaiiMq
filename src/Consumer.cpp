/**
 * @file Consumer.cpp
 * @author ayano
 * @date 1/25/24
 * @brief
*/

#include "Consumer.h"

namespace messaging {

    messaging::Consumer::Consumer(std::vector <Topic> topics) : subscribed(std::move(topics)) {

    }

    void Consumer::subscribe(const Topic &topic) {
        if (std::find(subscribed.begin(), subscribed.end(), topic) == subscribed.end()) {
            subscribed.push_back(topic);
        }
        else {
            throw std::runtime_error("Attempting to subscribe a subscribed topic! Topic: " + topic.getName());
        }
    }

    void Consumer::unsubscribe(const Topic &topic) {
        auto it = std::find(subscribed.begin(), subscribed.end(), topic);
        if (it != subscribed.end()) {
            subscribed.erase(it);
        }
        else {
            throw std::runtime_error("Attempting to unsubscribe a non-subscribed topic! Topic: " + topic.getName());
        }
    }




} // messaging