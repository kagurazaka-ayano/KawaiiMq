/**
 * @file Queue.cpp
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#include "Queue.h"

namespace messaging {
    std::shared_ptr<IMessage> Queue::wait() {
        mtxunique lock(mtx);
        cond.wait(lock, [this]{return !queue.empty();});
        auto ret = std::make_shared<IMessage>(std::move(queue.front()));
        queue.pop();
        return ret;
    }

    void Queue::push(const IMessage &msg) {
        mtxguard lock(mtx);
        queue.push(msg);
        cond.notify_one();
    }

    void Queue::push(IMessage &&msg) noexcept {
        mtxguard lock(mtx);
        queue.push(msg);
        cond.notify_one();
    }

    std::size_t Queue::size() const noexcept {
        return queue.size();
    }

    bool Queue::empty() const noexcept {
        return queue.empty();
    }

    Queue::Queue(const Queue &other): queue(other.queue) {
    }

    Queue::Queue(Queue &&other) noexcept: queue(std::move(other.queue)) {
    }

    bool Queue::operator==(const Queue &other) {
        return this == &other;
    }

    Queue &Queue::operator=(const Queue &other) {
        if(this != &other) {
            queue = other.queue;
        }
        return *this;
    }

    Queue &Queue::operator=(Queue &&other) noexcept {
        if(this != &other) {
            queue = std::move(other.queue);
        }
        return *this;
    }

    template<class... Args>
    void Queue::emplace(Args &&... args) {
        mtxguard lock(mtx);
        queue.emplace(args...);
        cond.notify_one();
    }
}

