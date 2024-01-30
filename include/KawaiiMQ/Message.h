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

    class ISerializable {
    public:
        virtual ~ISerializable() = default;

        virtual std::string serialize() const = 0;

        virtual void deserialize(const std::string &data) = 0;
    };

    class MessageData : public ISerializable {
    public:
        virtual ~MessageData() = default;
    };

    template<typename T> requires std::is_base_of_v<ISerializable, T> || std::is_fundamental_v<T>
    class Message {
    public:
        explicit Message(T content) : content(std::move(content)) {}

        T getContent() const {
            return content;
        }

        void setContent(T content) {
            this->content = std::move(content);
        }

        template<typename U>
        requires std::is_base_of_v<ISerializable, U> || std::is_fundamental_v<U>
        U getMessage(std::shared_ptr<MessageData> in) {
            std::shared_ptr<Message<T>> tmp = std::dynamic_pointer_cast<Message<T>>(in);
            if (tmp) {
                return tmp->val;
            }
            throw TypeException(
                    "Expected type " + std::string(typeid(T).name()) + ", got " + std::string(typeid(in).name()));
        }

    private:
        T content;
    };

    template<typename T>
    requires std::is_base_of_v<ISerializable, T> || std::is_fundamental_v<T>
    std::shared_ptr<Message<T>> makeMessage(T content) {
        auto msg = std::make_shared<Message<T>>(content);
        return msg;
    }
}
#endif //KAWAIIMQ_MESSAGE_H
