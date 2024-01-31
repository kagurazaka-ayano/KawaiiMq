/**
 * @file Consumer.cpp
 * @author ayano
 * @date 1/29/24
 * @brief
*/

#include "Consumer.h"

namespace KawaiiMQ {

    Consumer::Consumer(const std::vector<Topic> &topics) : subscribed(std::move(topics)) {

    }

    void Consumer::subscribe(const Topic &topic) {
        std::lock_guard lock(mtx);
        auto manager = MessageQueueManager::Instance();
        if (!manager->isRelatedAny(topic)) {
            throw TopicException("Attempting to subscribe a topic not related to any queue! Topic: " + topic.getName());
        }
        if (std::find(subscribed.begin(), subscribed.end(), topic) == subscribed.end()) {
            subscribed.push_back(topic);
        }
        else {
            throw TopicException("Attempting to subscribe a subscribed topic! Topic: " + topic.getName());
        }
    }

    void Consumer::unsubscribe(const Topic &topic) {
        std::lock_guard lock(mtx);
        auto it = std::find(subscribed.begin(), subscribed.end(), topic);
        if (it != subscribed.end()) {
            subscribed.erase(it);
        }
        else {
            throw TopicException("Attempting to unsubscribe a non-subscribed topic! Topic: " + topic.getName());
        }
    }

    std::unordered_map<Topic, std::vector<std::shared_ptr<MessageData>>> Consumer::fetchMessage() {
        auto manager = MessageQueueManager::Instance();
        std::unordered_map<Topic, std::vector<std::shared_ptr<MessageData>>> ret;
        for(const auto& i : subscribed) {
            auto queue = manager->getAllRelatedQueue(i);
            for(auto& j : queue) {
                auto message = j->wait();
                {
                    std::cout << "fetched" << std::endl;
                    ret[i].push_back(message);
                }
            }
        }
        std::cout << "fetcher ok" << std::endl;
        return ret;
    }

    std::vector<std::shared_ptr<MessageData>> Consumer::fetchSingleTopic(const Topic &topic) {
        if(std::find(subscribed.begin(), subscribed.end(), topic) == subscribed.end()) {
            throw TopicException("topic not subscribed");
        }
        auto manager = MessageQueueManager::Instance();
        std::vector<std::shared_ptr<MessageData>> ret;
        auto queue = manager->getAllRelatedQueue(topic);
        for (auto &j : queue) {
            if (j->empty()) {
                throw QueueException("queue empty");
            }
            auto message = j->wait();
            ret.push_back(message);
        }
        return ret;
    }

    Consumer::Consumer(const std::string &name): name(name) {

    }

    std::string Consumer::getName() const {
        return name;
    }

    std::vector<Topic> Consumer::getSubscribedTopics() const {
        return subscribed;
    }

}
