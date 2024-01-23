/**
 * @file Queue.h
 * @author ayano
 * @date 1/22/24
*/

#ifndef MESSAGEQUEUE_QUEUE_H
#define MESSAGEQUEUE_QUEUE_H

#include <condition_variable>
#include <shared_mutex>
#include <queue>
#include "IMessage.h"

/**
 * messaging namespace
 */
namespace messaging {
    using mtxguard = std::lock_guard<std::shared_mutex>;
    using mtxshared = std::shared_lock<std::shared_mutex>;
    using mtxunique = std::unique_lock<std::shared_mutex>;
    /**
     * A message queue that supports basic queue operation, and notification-based value fetch
     */
    class Queue {
    public:
        /**
         * Constructor
         */
        Queue() = default;

        /**
         * Wait for the result when called. Will return a std::shared_ptr containing message and pop the message if notified.
         * @return
         */
        std::shared_ptr<IMessage> wait();

        /**
         * Push a message to the queue
         * @param msg message pushing in
         */
        void push(const IMessage& msg);

        /**
         * Push a message to the queue
         * @param msg message pushing in
         */
        void push(IMessage&& msg) noexcept;

        /**
         * Given all the params, construct an object in place and insert it
         * @param args Constructor
         */
        template<class... Args>
        void emplace(Args&&... args);

        /**
         * Size of the queue
         * @return size of the queue
         */
        std::size_t size() const noexcept;

        /**
         * Check if queue is empty
         * @return false if not empty, true if empty
         */
        [[nodiscard]] bool empty() const noexcept;

    private:
        mutable std::shared_mutex mtx;
        std::queue<IMessage> queue;
        std::condition_variable_any cond;
    };
}


#endif //MESSAGEQUEUE_QUEUE_H
