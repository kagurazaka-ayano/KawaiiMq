/**
 * @file Client.cpp
 * @author ayano
 * @date 1/25/24
 * @brief
*/

#include "Client.h"

namespace messaging {

    messaging::Client::Client(std::vector <Topic> topics) : subscribed(std::move(topics)) {

    }

    void Client::subscribe(const Topic &topic) {
        if (std::find_if(subscribed.begin(), subscribed.end(), [&](const auto &item) {
            return &topic == &item;
        }) == subscribed.end()) {
            subscribed.push_back(topic);
        }
        else {
            std::cerr << "Duplicated topic subscription! Topic: " << topic.getName() << std::endl;
        }
    }

    void Client::unsubscribe(const Topic &topic) {
        if (std::find_if(subscribed.begin(), subscribed.end(), [&](const auto &item) {
            return &topic == &item;
        }) == subscribed.end()) {
            std::cerr << "Attempting to unsubscribe a not-subscribed topic! Topic: " << topic.getName() << std::endl;
        }
        else {
            std::remove(subscribed.begin(), subscribed.end(), topic);
        }
    }




} // messaging