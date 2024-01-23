/**
 * @file IMessage.h
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#ifndef MESSAGEQUEUE_IMESSAGE_H
#define MESSAGEQUEUE_IMESSAGE_H
#include <string>

namespace messaging {
    /**
     * Interface that all messages should composite
     * @tparam T type of content this message contains
     */
    template<typename T>
    class IMessage {
    public:
        /**
         * get content of the message
         * @return content
         */
        virtual T getContent() = 0;

        /**
         * set content of the message
         * @param content given content
         */
        virtual void setContent(const T& content) = 0;

        /**
         * set content of the message
         * @param content given content
         */
        virtual void setContent(T&& content) noexcept = 0;
    };
}

#endif //MESSAGEQUEUE_IMESSAGE_H
