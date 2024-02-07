/**
 * @file utility.cpp
 * @author ayano
 * @date 1/23/24
 * @brief Utility functions
*/


#ifndef KAWAIIMQ_UTILITY_H
#define KAWAIIMQ_UTILITY_H

#include "Message.h"

namespace KawaiiMQ{

    /**
     * A concept that checks if a type is a message
     * @tparam C any type
     */
    template <class C>
    concept MessageType = requires(C c) {
        []<typename X>(Message<X>&){}(c);
    };

}

#endif //KAWAIIMQ_UTILITY_H

