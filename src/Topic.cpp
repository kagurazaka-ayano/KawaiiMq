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
}