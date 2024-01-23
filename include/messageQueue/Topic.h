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
#include "Queue.h"

namespace messaging{

    class Topic {
    public:
        explicit Topic(std::string  name);
        Topic(const Topic& other);
        Topic(Topic&& other) noexcept;
        [[nodiscard]] std::string getName() const;
        bool operator==(const Topic& other) const {
            return name == other.getName();
        }
    private:
        std::string name;
    };


}
template<>
struct std::hash<messaging::Topic>{
    std::size_t operator()(const messaging::Topic& topic) const;
};

std::size_t std::hash<messaging::Topic>::operator()(const messaging::Topic &topic) const  {
    return (std::hash<string>()(topic.getName()) << 1) ^ (std::hash<string>()(topic.getName()) >> 1);
}

#endif //MESSAGEQUEUE_TOPIC_H
