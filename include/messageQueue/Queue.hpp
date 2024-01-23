/**
 * @file Queue.h
 * @author ayano
 * @date 1/22/24
*/

#ifndef MESSAGEQUEUE_QUEUE_HPP
#define MESSAGEQUEUE_QUEUE_HPP

#include <condition_variable>
#include <shared_mutex>
#include <queue>
#include <utility>
#include "IMessage.h"
#include "utility.hpp"

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
    requires DerivedFromTemplate<IMessage, T>
    class Queue {
    public:

        Queue() = default;

        explicit Queue(std::string name);

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

        std::shared_ptr<T> wait();

        /**
         * Push a message to the queue
         * @param msg message pushing in
         * @tparam T message content type
         */
        void push(const T& msg);

        /**
         * Push a message to the queue
         * @param msg message pushing in
         * @tparam T message content type
         */

        void push(T&& msg) noexcept;

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
        std::queue<T> queue;
        mutable std::condition_variable_any cond;
        std::string name;
    };

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    Queue<T>::Queue(std::string name): name(std::move(name)) {

    }
    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::shared_ptr<T> Queue<T>::wait() {
        mtxunique lock(mtx);
        cond.wait(lock, [this]{return !queue.empty();});
        auto ret = std::make_shared<T>(std::move(queue.front()));
        queue.pop();
        return ret;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void Queue<T>::push(const T& msg) {
        mtxguard lock(mtx);
        queue.push(msg);
        cond.notify_one();
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    void Queue<T>::push(T &&msg) noexcept {
        mtxguard lock(mtx);
        queue.push(msg);
        cond.notify_one();
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::size_t Queue<T>::size() const noexcept {
        mtxshared lock(mtx);
        return queue.size();
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    bool Queue<T>::empty() const noexcept {
        mtxshared lock(mtx);
        return queue.empty();
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    Queue<T>::Queue(const Queue<T> &other): queue(other.queue) {
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    Queue<T>::Queue(Queue<T> &&other) noexcept: queue(std::move(other.queue)) {
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    bool Queue<T>::operator==(const Queue<T> &other) {
        mtxshared lock(mtx);
        return this == &other;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    Queue<T> &Queue<T>::operator=(const Queue<T> &other) {
        mtxguard lock(mtx);
        if(this != &other) {
            queue = other.queue;
        }
        return *this;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    Queue<T> &Queue<T>::operator=(Queue<T> &&other) noexcept {
        if(this != &other) {
            queue = std::move(other.queue);
        }
        return *this;
    }
}


#endif //MESSAGEQUEUE_QUEUE_HPP
