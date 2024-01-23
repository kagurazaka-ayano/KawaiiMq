/**
 * @file Queue.cpp
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#include "Queue.h"

namespace messaging {
    template<typename T>
    std::shared_ptr<IMessage<T>> Queue<T>::wait() {
        mtxunique lock(mtx);
        cond.wait(lock, [this]{return !queue.empty();});
        auto ret = std::make_shared<IMessage>(std::move(queue.front()));
        queue.pop();
        return ret;
    }

    template<typename T>
    void Queue<T>::push(const IMessage<T>& msg) {
        mtxguard lock(mtx);
        queue.push(msg);
        cond.notify_one();
    }

    template<typename T>
    void Queue<T>::push(IMessage<T> &&msg) noexcept {
        mtxguard lock(mtx);
        queue.push(msg);
        cond.notify_one();
    }

    template<typename T>
    std::size_t Queue<T>::size() const noexcept {
        return queue.size();
    }

    template<typename T>
    bool Queue<T>::empty() const noexcept {
        return queue.empty();
    }

    template<typename T>
    Queue<T>::Queue(const Queue<T> &other): queue(other.queue) {
    }

    template<typename T>
    Queue<T>::Queue(Queue<T> &&other) noexcept: queue(std::move(other.queue)) {
    }

    template<typename T>
    bool Queue<T>::operator==(const Queue<T> &other) {
        return this == &other;
    }

    template<typename T>
    Queue<T> &Queue<T>::operator=(const Queue<T> &other) {
        if(this != &other) {
            queue = other.queue;
        }
        return *this;
    }

    Queue<T> &Queue<T>::operator=(Queue<T> &&other) noexcept {
        if(this != &other) {
            queue = std::move(other.queue);
        }
        return *this;
    }

    template<class... Args>
    void Queue<T>::emplace(Args &&... args) {
        mtxguard lock(mtx);
        queue.emplace(args...);
        cond.notify_one();
    }
}

