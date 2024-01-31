/**
 * @file Queue.h
 * @author ayano
 * @date 1/22/24
 * @brief A message queue that supports basic queue operation, and notification-based value fetch
*/

#ifndef KAWAIIMQ_QUEUE_H
#define KAWAIIMQ_QUEUE_H

#include <condition_variable>
#include <shared_mutex>
#include <queue>
#include <iostream>
#include <utility>
#include <limits>
#include "Message.h"
#include "utility.hpp"
#include "Exceptions.h"
/**
 * KawaiiMQ namespace
 */
namespace KawaiiMQ {

    /**
     * A message queue that supports basic queue operation, and notification-based value fetch
     */
    class Queue {
        // convenience alias
        using mtxsharedguard = std::lock_guard<std::shared_mutex>;
        using mtxshared = std::shared_lock<std::shared_mutex>;
    public:

        Queue() = default;

        explicit Queue(std::string name);

        Queue(const Queue& other);

        Queue(Queue&& other) noexcept;

        bool operator==(const Queue& other);

        Queue& operator=(const Queue& other);

        Queue& operator=(Queue&& other) noexcept;

        /**
         * Wait for the result when called. Will pop the message if notified.
         * @return a std::shared_ptr containing message
         */
        std::shared_ptr<MessageData> wait();


        /**
         * Push a message to the queue
         * @param msg message pushing in
         * @tparam T message content type
         */
        template<MessageType T>
        void push(const std::shared_ptr<T>& msg);

        /**
         * Push a message to the queue
         * @param msg message pushing in
         * @tparam T message content type
         */
        template<MessageType T>
        void push(std::shared_ptr<T>&& msg) noexcept;

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

        /**
         * Set timeout for wait()
         * @param timeout_ms timeout in milliseconds
         * @remark 0 means no timeout
         */
        void setTimeout(int timeout_ms) noexcept;

        int getTimeout() const noexcept;

        void setSafeTimeout(int timeout_ms) noexcept;

        int getSafeTimeout() const noexcept;

        std::condition_variable_any& getSafeCond() noexcept;

        std::string getName() const;

    private:
        static std::shared_mutex mtx;
        std::queue<std::shared_ptr<MessageData>> queue;
        mutable std::condition_variable_any cond;
        std::string name;
        int timeout_ms = 0;
        int safe_timeout_ms = 5000;
        mutable std::condition_variable_any safe_cond;
    };

    template<MessageType T>
    void Queue::push(const std::shared_ptr<T>& msg) {
        mtxsharedguard lock(mtx);
        queue.push(msg);
        cond.notify_one();
    }

    template<MessageType T>
    void Queue::push(std::shared_ptr<T> &&msg) noexcept {
        mtxsharedguard lock(mtx);
        queue.push(msg);
        cond.notify_one();
    }

}


#endif //KAWAIIMQ_QUEUE_H
