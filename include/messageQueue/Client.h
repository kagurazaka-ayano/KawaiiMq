/**
 * @file Client.h
 * @author ayano
 * @date 1/23/24
 * @brief
*/

#ifndef MESSAGEQUEUE_CLIENT_H
#define MESSAGEQUEUE_CLIENT_H

#include "MessageQueueManager.h"

namespace messaging {
    class IPublisher {
    public:

        virtual void publish(const IMessage& message, const Topic& topic) = 0;

        virtual void publish(IMessage&& message, Topic&& topic) noexcept = 0;
    };

    class IRetriever{
    public:

        virtual IMessage retrieve(const Topic& topic) = 0;

        virtual IMessage retrieve(Topic&& topic) = 0;
    };

} // messaging

#endif //MESSAGEQUEUE_CLIENT_H
