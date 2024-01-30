/**
 * @file Exceptions.cpp
 * @author ayano
 * @date 1/29/24
 * @brief
*/

#include "Exceptions.h"

namespace KawaiiMQ {

    QueueException::QueueException(const std::string &queueName) {
        this->message = queueName;
    }

    const char *QueueException::what() const noexcept {
        return message.c_str();
    }

    TopicException::TopicException(const std::string &message) {
        this->message = message;
    }

    const char *TopicException::what() const noexcept {
        return message.c_str();
    }

    TypeException::TypeException(const std::string &message) {
        this->message = message;
    }

    const char *TypeException::what() const noexcept {
        return message.c_str();
    }
}

