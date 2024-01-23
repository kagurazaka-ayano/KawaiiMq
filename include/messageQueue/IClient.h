/**
 * @file Client.h
 * @author ayano
 * @date 1/23/24
 * @brief
*/

#ifndef MESSAGEQUEUE_ICLIENT_H
#define MESSAGEQUEUE_ICLIENT_H

#include "MessageQueueManager.h"

namespace messaging {
    /**
     * interface all publisher should inherent
     * @tparam T type of message content this publisher is publishing
     */
    template<typename T>
    class IPublisher {
    public:
        /**
         * publish a message to a specific topic
         * @param message message pending publish
         * @param topic topic this message belongs to
         */
        virtual void publish(const IMessage<T>& message, const Topic& topic) = 0;

        /**
         * publish a message to a specific topic, but rvalue reference
         * @param message message pending publish
         * @param topic topic this message belongs to
         */
        virtual void publish(IMessage<T>&& message, Topic&& topic) noexcept = 0;
    };

    /**
     * interface all receiver should inherent
     * @tparam T type of message content this receiver is receiving
     */
    template<typename T>
    class IRetriever{
    public:
        /**
         * retrieve the first message from a topic
         * @param topic specified topic
         * @return the first message
         */
        virtual std::shared_ptr<IMessage<T>> retrieve(const Topic& topic) = 0;

    };

} // messaging

#endif //MESSAGEQUEUE_ICLIENT_H
