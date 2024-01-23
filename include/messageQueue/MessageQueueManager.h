/**
 * @file MessageQueueManager.h
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#ifndef MESSAGEQUEUE_MESSAGEQUEUEMANAGER_H
#define MESSAGEQUEUE_MESSAGEQUEUEMANAGER_H

#include "Queue.h"
#include "Topic.h"
#include <mutex>
#include <iostream>
#include <unordered_map>
namespace messaging {
    class MessageQueueManager {
    public:
        MessageQueueManager &operator=(MessageQueueManager &other) = delete;

        MessageQueueManager(MessageQueueManager &other) = delete;

        static std::shared_ptr<MessageQueueManager> Instance();

        void subscribe(const Topic& topic, const Queue& queue);

        void unsubscribe(const Topic& topic, const Queue& queue);

        void publish(const IMessage& message, const Topic& topic);

        [[nodiscard]] std::shared_ptr<IMessage> retrieve(Queue queue) const;

    private:
        MessageQueueManager() = default;

        static std::once_flag flag;
        static std::shared_ptr<MessageQueueManager> instance;
        std::unordered_map<Topic, std::vector<Queue>> topic_map;
        std::unordered_map<std::string, Topic> registered_topic;

    };
}
#endif //MESSAGEQUEUE_MESSAGEQUEUEMANAGER_H
