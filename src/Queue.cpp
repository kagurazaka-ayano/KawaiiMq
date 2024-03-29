/**
 * @file Queue.cpp
 * @author ayano
 * @date 1/30/24
 * @brief
*/

#include "Queue.h"

namespace KawaiiMQ {

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
        mtxsharedguard lock(mtx);
        if(this != &other) {
            name = std::move(other.name);
            queue = std::move(other.queue);
        }
        return *this;
    }

    void Queue::setTimeout(int timeout_ms) noexcept {
        mtxsharedguard lock(mtx);
        this->timeout_ms = timeout_ms;
    }

    int Queue::getTimeout() const noexcept {
        mtxshared lock(mtx);
        return timeout_ms;
    }

    void Queue::setSafeTimeout(int timeout_ms) noexcept {
        mtxsharedguard lock(mtx);
        this->safe_timeout_ms = timeout_ms;
    }

    int Queue::getSafeTimeout() const noexcept {
        mtxshared lock(mtx);
        return safe_timeout_ms;
    }

    std::condition_variable_any &Queue::getSafeCond() noexcept {
        mtxshared lock(mtx);
        return safe_cond;
    }

    std::shared_ptr<MessageData> Queue::wait() {
        std::unique_lock lock(mtx);
        if (timeout_ms == 0) {
            cond.wait(lock, [this](){return !queue.empty();});
        }
        else {
            cond.wait_for(lock, std::chrono::milliseconds(timeout_ms), [this](){return !queue.empty();});
            if(queue.empty()) {
                throw QueueException("queue fetch timeout");
            }
        }
        auto ret = queue.front();
        queue.pop();
        if (queue.empty()) {
            safe_cond.notify_all();
        }
        return ret;
    }

    bool Queue::tryWait(std::shared_ptr<MessageData>& msg) {
        std::unique_lock lock(mtx);
        if(queue.empty()) {
            return false;
        }
        if (timeout_ms == 0) {
            cond.wait(lock, [this](){return !queue.empty();});
        }
        else {
            cond.wait_for(lock, std::chrono::milliseconds(timeout_ms), [this](){return !queue.empty();});
            if(queue.empty()) {
                throw QueueException("queue fetch timeout");
            }
        }
        msg = queue.front();
        queue.pop();
        if (queue.empty()) {
            safe_cond.notify_all();
        }
        return true;
    }

    std::string Queue::getName() const {
        mtxshared lock(mtx);
        return name;
    }

    std::shared_ptr<Queue> makeQueue(const std::string& name) {
        return std::make_shared<Queue>(name);
    }

    std::shared_ptr<Queue> makeQueue(std::string&& name) {
        return std::make_shared<Queue>(std::move(name));
    }

    std::shared_ptr<Queue> makeQueue(const Queue& other) {
        return std::make_shared<Queue>(other);
    }

    std::shared_ptr<Queue> makeQueue(Queue&& other) {
        return std::make_shared<Queue>(std::move(other));
    }

}
