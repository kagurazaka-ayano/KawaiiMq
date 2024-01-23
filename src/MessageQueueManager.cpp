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
}
