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
     * @tparam T message content type
     */
    template<typename T>
    class Queue {
    public:

        Queue(const std::string& name);

        Queue(const Queue<T>& other);

        Queue(Queue<T>&& other) noexcept;

        bool operator==(const Queue<T>& other);

        Queue<T>& operator=(const Queue<T>& other);

        Queue<T>& operator=(Queue<T>&& other) noexcept;

        /**
         * Wait for the result when called. Will pop the message if notified.
         * @return a std::shared_ptr containing message
         * @tparam T message content type
         */

        std::shared_ptr<IMessage<T>> wait();

        /**
         * Push a message to the queue
         * @param msg message pushing in
         * @tparam T message content type
         */
        void push(const IMessage<T>& msg);

        /**
         * Push a message to the queue
         * @param msg message pushing in
         * @tparam T message content type
         */

        void push(IMessage<T>&& msg) noexcept;

        /**
         * Given all the params, construct an object in place and insert it
         * @param args Constructor
         * @tparam T message content type
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
        std::queue<IMessage<T>> queue;
        mutable std::condition_variable_any cond;
        std::string name;
    };
}


#endif //MESSAGEQUEUE_QUEUE_H
