/**
 * @file IMessage.h
 * @author ayano
 * @date 1/22/24
 * @brief The message interface and other useful message types
*/

#ifndef KAWAIIMQ_MESSAGE_H
#define KAWAIIMQ_MESSAGE_H
#include <string>
#include <memory>
#include "Exceptions.h"

namespace KawaiiMQ {

    class MessageData {
    public:
        MessageData() = default;
        virtual ~MessageData() = default;
    };

    template<typename T>
    class Message : public MessageData {
    public:
        explicit Message(T content) : content(std::move(content)) {}

        void setContent(T content) {
            this->content = std::move(content);
        }

        template<typename U>
        friend U getMessage(std::shared_ptr<MessageData> in);

    private:
        T content;
    };

    template<typename T>
    T getMessage(std::shared_ptr<MessageData> in) {
        std::shared_ptr<Message<T>> tmp = std::dynamic_pointer_cast<Message<T>>(in);
        if (tmp != nullptr) {
            return tmp->content;
        }
        throw TypeException(
                "Expected type " + std::string(typeid(T).name()) + ", got " + std::string(typeid(in).name()));
    }

    template<typename T>
    std::shared_ptr<Message<T>> makeMessage(T content) {
        auto msg = std::make_shared<Message<T>>(content);
        return msg;
    }
}
#endif //KAWAIIMQ_MESSAGE_H
