/**
 * @file Producer.hpp
 * @author ayano
 * @date 1/27/24
 * @brief A producer of the message queue
*/

#ifndef KAWAIIMQ_PRODUCER_HPP
#define KAWAIIMQ_PRODUCER_HPP

#include "Topic.h"
#include "IMessage.hpp"
#include "MessageQueueManager.hpp"

namespace messaging {
    /**
     * Producer is a class that can publish message to a topic
     * @tparam T type of message this consumer accepts
     */
    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    class Producer {
    public:
        /**
         * publish a message to a topic
         * @param topic topic you want to publish
         * @param message message you want to publish
         */
        void publishMessage(const Topic& topic, T message) {
            auto manager = MessageQueueManager<T>::Instance();
            auto queues = manager->getAllRelatedQueue(topic);
            for(auto& queue : queues) {
                queue.get().push(std::move(message));
            }
        }

        /**
         * broadcast a message to all topics
         * @param message message you want to broadcast
         */
        void broadcastMessage(T message) {
            auto manager = MessageQueueManager<T>::Instance();
            auto queues = manager->getAllRelatedQueue();
            for(auto& queue : queues) {
                queue.get().push(std::move(message));
            }
        }
    };
}

#endif //KAWAIIMQ_PRODUCER_HPP
