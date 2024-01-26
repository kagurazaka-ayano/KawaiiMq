/**
 * @file Client.h
 * @author ayano
 * @date 1/25/24
 * @brief
*/

#ifndef KAWAIIMQ_CLIENT_H
#define KAWAIIMQ_CLIENT_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "Topic.h"
#include "IMessage.h"
#include "utility.hpp"
#include "MessageQueueManager.hpp"

namespace messaging {

    class Client {
    public:
        Client() = default;

        explicit Client(std::vector<Topic> topics);

        void subscribe(const Topic& topic);

        void unsubscribe(const Topic& topic);

        template<typename T>
        requires DerivedFromTemplate<IMessage, T>
        std::unordered_map<Topic, std::vector<std::shared_ptr<T>>> fetchMessage();

        template<typename T>
        requires DerivedFromTemplate<IMessage, T>
        void publishMessage(const T& message);

        template<typename T>
        requires DerivedFromTemplate<IMessage, T>
        std::vector<std::shared_ptr<T>> fetchSingleTopic(const Topic& topic);

        template<typename T>
        requires DerivedFromTemplate<IMessage, T>
        void publishSingleMessage(const Topic& topic, const T& message);

    private:
        static std::mutex mtx;
        std::vector<Topic> subscribed;
    };


    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::unordered_map<Topic, std::vector<std::shared_ptr<T>>> Client::fetchMessage() {
        std::lock_guard lock(mtx);
        auto manager = MessageQueueManager<T>::Instance();
        std::unordered_map<Topic, std::vector<std::shared_ptr<T>>> ret;
        for(const auto& i : subscribed) {
            auto queue = manager->getAllRelatedQueue(i);
            for(auto& j : queue) {
                auto message = j.get().forcePop();
                ret[i].push_back(message);
            }
        }
        return ret;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void Client::publishMessage(const T& message) {
        std::lock_guard lock(mtx);
        auto manager = MessageQueueManager<T>::Instance();
        for (const auto& i: subscribed) {
            auto queue = manager->getAllRelatedQueue(i);
            for (auto& j : queue) {
                j.get().push(message);
            }
        }
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::vector<std::shared_ptr<T>> Client::fetchSingleTopic(const Topic &topic) {
        std::lock_guard lock(mtx);
        auto manager = MessageQueueManager<T>::Instance();
        std::vector<std::shared_ptr<T>> ret;
        auto queue = manager->getAllRelatedQueue(topic);
        for (auto &j : queue) {
            auto message = j.get().forcePop();
            ret.push_back(message);
        }
        return ret;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void Client::publishSingleMessage(const Topic &topic, const T &message) {
        std::lock_guard lock(mtx);
        auto manager = MessageQueueManager<T>::Instance();
        auto queue = manager->getAllRelatedQueue(topic);
        for (auto &i : queue) {
            i.get().push(message);
        }
    }

} // messaging

#endif //KAWAIIMQ_CLIENT_H
