/**
 * @file Topic.h
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#ifndef MESSAGEQUEUE_TOPIC_H
#define MESSAGEQUEUE_TOPIC_H

#include <string>
#include <vector>
#include "Queue.hpp"

namespace messaging{

    class Topic {
    public:
        explicit Topic(std::string name);
        Topic(const Topic& other);
        Topic(Topic&& other) noexcept;
        /**
         * get topic name
         * @return topic name
         */
        [[nodiscard]] std::string getName() const;
        bool operator==(const Topic& other) const;
    private:
        std::string name;
    };
}
template<>
struct std::hash<messaging::Topic>{
    std::size_t operator()(const messaging::Topic& topic) const;
};



#endif //MESSAGEQUEUE_TOPIC_H
