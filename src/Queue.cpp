/**
 * @file Queue.cpp
 * @author ayano
 * @date 1/30/24
 * @brief
*/

#include "Queue.h"

namespace KawaiiMQ {

    std::shared_mutex Queue::mtx;

    Queue::Queue(std::string name) : name(std::move(name)) {

    }


    std::size_t Queue::size() const noexcept {
        mtxshared lock(mtx);
        return queue.size();
    }

    bool Queue::empty() const noexcept {
        mtxshared lock(mtx);
        return queue.empty();
    }

    Queue::Queue(const Queue &other): queue(other.queue), name(other.name) {
    }

    Queue::Queue(Queue &&other) noexcept: queue(std::move(other.queue)), name(std::move(other.name)) {

    }

    bool Queue::operator==(const Queue &other) {
        mtxshared lock(mtx);
        return this == &other;
    }


    Queue &Queue::operator=(const Queue &other) {
        mtxsharedguard lock(mtx);
        if(this != &other) {
            name = other.name;
            queue = other.queue;
        }
        return *this;
    }

    Queue &Queue::operator=(Queue &&other) noexcept {
        if(this != &other) {
            name = std::move(other.name);
            queue = std::move(other.queue);
        }
        return *this;
    }

    void Queue::setTimeout(int timeout_ms) noexcept {
        this->timeout_ms = timeout_ms;
    }

    int Queue::getTimeout() const noexcept {
        return timeout_ms;
    }

    void Queue::setSafeTimeout(int timeout_ms) noexcept {
        this->safe_timeout_ms = timeout_ms;
    }

    int Queue::getSafeTimeout() const noexcept {
        return safe_timeout_ms;
    }

    std::condition_variable_any &Queue::getSafeCond() noexcept {
        return safe_cond;
    }
}
