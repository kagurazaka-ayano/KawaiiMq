/**
 * @file IMessage.h
 * @author ayano
 * @date 1/22/24
 * @brief The message interface and other useful message types
*/

#ifndef KAWAIIMQ_IMESSAGE_HPP
#define KAWAIIMQ_IMESSAGE_HPP
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
         * for other caller to find type;
         */
        using TYPE = T;

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

    /*
     * A specialized message that contains a string
     */
    class StringMessage : public messaging::IMessage<std::string> {
    public:
        StringMessage(const std::string &content): content(content) {

        }

        StringMessage(std::string &&content): content(std::move(content)) {

        }

        std::string getContent() override {
            return content;
        }

        void setContent(const std::string &content) override {
            this->content = content;
        }

        void setContent(std::string &&content) noexcept override {
            this->content = std::move(content);
        }

    private:
        std::string content;
    };

    /*
     * A specialized message that contains an int
     */
    class IntMessage : public messaging::IMessage<int> {
    public:
        IntMessage(int content): content(content) {

        }

        int getContent() override {
            return content;
        }

        void setContent(const int& content) override {
            this->content = content;
        }

        void setContent(int &&content) noexcept override {
            this->content = content;
        }

    private:
        int content;
    };

    /*
     * A specialized message that contains a double
     */
    class DoubleMessage : public messaging::IMessage<double> {
    public:
        DoubleMessage(double content): content(content) {

        }

        double getContent() override {
            return content;
        }

        void setContent(const double& content) override {
            this->content = content;
        }

        void setContent(double &&content) noexcept override {
            this->content = content;
        }
    private:
        double content;
    };
}

#endif //KAWAIIMQ_IMESSAGE_HPP
