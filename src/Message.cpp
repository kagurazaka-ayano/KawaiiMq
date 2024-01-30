/**
 * @file Message.cpp
 * @author ayano
 * @date 1/29/24
 * @brief
*/

#include "Message.h"

namespace KawaiiMQ {

    template<typename T>
    requires std::is_base_of_v<ISerializable, T> || std::is_fundamental_v<T>
    std::shared_ptr<Message<T>> makeMessage(T content) {
        auto msg = std::make_shared<Message<T>>(content);
        return msg;
    }
}
