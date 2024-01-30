/**
 * @file IMessage.h
 * @author ayano
 * @date 1/22/24
 * @brief The message interface and other useful message types
*/

#ifndef KAWAIIMQ_MESSAGE_H
#define KAWAIIMQ_MESSAGE_H
#include <string>

namespace KawaiiMQ {

    class ISerializable {

    }

    class MessageData {
    public:
        virtual ~MessageData() = default;
    };
}

#endif //KAWAIIMQ_MESSAGE_H
