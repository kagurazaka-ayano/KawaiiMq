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
    /**
     * base class of message
     */
    class MessageData {
    public:
        MessageData() = default;
        virtual ~MessageData() = default;
    };

    /**
     * generic message wrapper
     * @tparam T type of message content
     */
    template<typename T>
    class Message : public MessageData {
    public:
        explicit Message(T content) : content(std::move(content)) {}

        /**
         * set content of the message
         * @param content content
         */
        void setContent(T content) {
            this->content = std::move(content);
        }

        template<typename U>
        friend U getMessage(std::shared_ptr<MessageData> in);

    private:
        T content;
    };

    /**
     * Get the message content from a message
     * @tparam T type of message you are expecting
     * @param in shared ptr of message
     * @return message content
     * @exception TypeException Will throw if the type of the message is not the same as the type you are expecting
     */
    template<typename T>
    T getMessage(std::shared_ptr<MessageData> in) {
        std::shared_ptr<Message<T>> tmp = std::dynamic_pointer_cast<Message<T>>(in);
        if (tmp != nullptr) {
            return tmp->content;
        }
        throw TypeException(
                "Expected type " + std::string(typeid(T).name()) + ", got " + std::string(typeid(in).name()));
    }

    /**
     * construct a message shared ptr
     * @tparam T type of content
     * @param content message content
     * @return message shared ptr
     */
    template<typename T>
    std::shared_ptr<Message<T>> makeMessage(T content) {
        auto msg = std::make_shared<Message<T>>(content);
        return msg;
    }
}
#endif //KAWAIIMQ_MESSAGE_H
