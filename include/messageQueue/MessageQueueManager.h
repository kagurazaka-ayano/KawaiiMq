/**
 * @file MessageQueueManager.h
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#ifndef MESSAGEQUEUE_MESSAGEQUEUEMANAGER_H
#define MESSAGEQUEUE_MESSAGEQUEUEMANAGER_H

#include "Queue.h"
#include <mutex>
#include <unordered_map>
namespace messaging {
    class MessageQueueManager {
    public:
        MessageQueueManager &operator=(MessageQueueManager &other) = delete;

        MessageQueueManager(MessageQueueManager &other) = delete;

        static std::shared_ptr<MessageQueueManager> Instance();

    private:
        MessageQueueManager() = default;

        static std::once_flag flag;
        static std::shared_ptr<MessageQueueManager> instance;
        std::unordered_map<MessageQueue, > message_subscriber;

    };
}
#endif //MESSAGEQUEUE_MESSAGEQUEUEMANAGER_H
