/**
 * @file Queue.h
 * @author ayano
 * @date 1/22/24
*/

#ifndef KAWAIIMQ_QUEUE_HPP
#define KAWAIIMQ_QUEUE_HPP

#include <condition_variable>
#include <shared_mutex>
#include <queue>
#include <iostream>
#include <utility>
#include "IMessage.h"
#include "utility.hpp"
/**
 * messaging namespace
 */
namespace messaging {


    using mtxguard = std::lock_guard<std::mutex>;
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
        static std::mutex mtx;
        std::queue<T> queue;
        mutable std::condition_variable cond;
        std::string name;
    };

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::mutex Queue<T>::mtx;

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    Queue<T>::Queue(std::string name): name(std::move(name)) {

    }
    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    std::shared_ptr<T> Queue<T>::wait() {
        std::unique_lock lock(mtx);
        cond.wait_for(lock, timeout, [this](){return !queue.empty();});
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
        mtxguard lock(mtx);
        return queue.size();
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    bool Queue<T>::empty() const noexcept {
        mtxguard lock(mtx);
        return queue.empty();
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    Queue<T>::Queue(const Queue<T> &other): queue(other.queue), name(other.name) {
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    Queue<T>::Queue(Queue<T> &&other) noexcept: queue(std::move(other.queue)), name(std::move(other.name)) {
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    bool Queue<T>::operator==(const Queue<T> &other) {
        mtxguard lock(mtx);
        return this == &other;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    Queue<T> &Queue<T>::operator=(const Queue<T> &other) {
        mtxguard lock(mtx);
        if(this != &other) {
            name = other.name;
            queue = other.queue;
        }
        return *this;
    }

    template<typename T>
    requires DerivedFromTemplate<IMessage, T>
    Queue<T> &Queue<T>::operator=(Queue<T> &&other) noexcept {
        if(this != &other) {
            name = std::move(other.name);
            queue = std::move(other.queue);
        }
        return *this;
    }
}


#endif //KAWAIIMQ_QUEUE_HPP
