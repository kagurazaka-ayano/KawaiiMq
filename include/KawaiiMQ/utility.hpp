/**
 * @file utility.cpp
 * @author ayano
 * @date 1/23/24
 * @brief Utility functions
*/


#ifndef KAWAIIMQ_UTILITY_H
#define KAWAIIMQ_UTILITY_H

#include <type_traits>
#include <chrono>
#include "Message.h"

namespace KawaiiMQ{

    template <class C>
    concept MessageType = requires(C c) {
        // IILE, that only binds to A<...> specialisations
        // Including classes derived from them
        []<typename X>(Message<X>&){}(c);
    };

}

#endif //KAWAIIMQ_UTILITY_H

