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

namespace messaging{

    template<template<typename...> class Base, typename Derived>
    struct IsDerivedFromTemplate {
        template<typename... Args>
        static constexpr bool Test(const Base<Args...> *) {
            return std::is_base_of<Base<Args...>, Derived>::value;
        }

        static constexpr bool Test(...) {
            return false;
        }

        static constexpr bool value = Test(static_cast<Derived *>(nullptr));
    };

    template<template<typename...> class Base, typename Derived>
    concept DerivedFromTemplate = IsDerivedFromTemplate<Base, Derived>::value;

}

#endif //KAWAIIMQ_UTILITY_H

