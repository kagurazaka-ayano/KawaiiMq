/**
 * @file Topic.cpp
 * @author ayano
 * @date 1/22/24
 * @brief
*/

#include "Topic.h"
#include <utility>


namespace messaging{


    Topic::Topic(std::string name): name(std::move(name)) {

    }

    Topic::Topic(const Topic &other): name(other.name) {

    }

    Topic::Topic(Topic &&other) noexcept: name(std::move(other.name)) {

    }

    std::string Topic::getName() const {
        return name;
    }

    bool Topic::operator==(const Topic &other) const {
        return name == other.getName();
    }

}
std::size_t std::hash<messaging::Topic>::operator()(const messaging::Topic &topic) const  {
    return (std::hash<string>()(topic.getName()) << 1) ^ (std::hash<string>()(topic.getName()) >> 1);
}