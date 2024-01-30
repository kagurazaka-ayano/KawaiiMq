/**
 * @file Topic.h
 * @author ayano
 * @date 1/22/24
 * @brief A topic that can be used to relate queues and subscribed by clients
*/


#ifndef KAWAIIMQ_TOPIC_H
#define KAWAIIMQ_TOPIC_H

#include <string>
#include <vector>
#include "Queue.h"

namespace KawaiiMQ{

    class Topic {
    public:
        explicit Topic(std::string name);
        Topic(const Topic& other);
        Topic(Topic&& other) noexcept;
        Topic& operator=(const Topic& other);
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
struct std::hash<KawaiiMQ::Topic>{
    std::size_t operator()(const KawaiiMQ::Topic& topic) const;
};



#endif //KAWAIIMQ_TOPIC_H
