/**
 * @file Exceptions.h
 * @author ayano
 * @date 1/29/24
 * @brief
*/

#ifndef KAWAIIMQ_EXCEPTIONS_H
#define KAWAIIMQ_EXCEPTIONS_H

#include <exception>
#include <string>

namespace KawaiiMQ {


    class QueueException : public std::exception {
    public:
        explicit QueueException(const std::string& message);
        [[nodiscard]] const char *what() const noexcept override;
    private:
        std::string message;
    };

    class TopicException : public std::exception {
    public:
        explicit TopicException(const std::string& message);
        [[nodiscard]] const char *what() const noexcept override;
    private:
        std::string message;
    };

    class TypeException : public std::exception {
    public:
        explicit TypeException(const std::string& message);
        [[nodiscard]] const char *what() const noexcept override;
    private:
        std::string message;
    };

}

#endif //KAWAIIMQ_EXCEPTIONS_H