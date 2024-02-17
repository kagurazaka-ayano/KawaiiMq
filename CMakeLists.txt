cmake_minimum_required(VERSION 3.27)
project(KawaiiMQ LANGUAGES CXX)
include(CTest)
include(FetchContent)
set(CMAKE_CXX_STANDARD 23)



set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


file(GLOB SRC "./src/*.cpp")
set(INCLUDE "./include/KawaiiMQ")
include_directories(${INCLUDE})
add_library(KawaiiMQ STATIC ${SRC})
set(PACKAGE_FILES ${SRC} ${INCLUDE} CMakeLists.txt LICENSE)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/KawaiiMQ-src.zip
        COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_CURRENT_BINARY_DIR}/KawaiiMQ-src.zip --format=zip -- ${PACKAGE_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${PACKAGE_FILES})
add_custom_target(KawaiiMQ_package DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/KawaiiMQ-src.zip)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()
file(GLOB OperatorTest "./src/test/OperatorTest/*.cpp")
add_executable(OperatorTest ${OperatorTest} ${SRC})
target_link_libraries(OperatorTest PRIVATE gtest_main gmock_main)
add_test(NAME OperatorTest COMMAND OperatorTest)
gtest_discover_tests(OperatorTest)
target_compile_definitions(OperatorTest PRIVATE -DTEST)

file(GLOB QueueTest "./src/test/QueueTest/*.cpp")
add_executable(QueueTest ${QueueTest} ${SRC})
target_link_libraries(QueueTest PRIVATE gtest_main gmock_main)
add_test(NAME QueueTest COMMAND QueueTest)
gtest_discover_tests(QueueTest)
target_compile_definitions(QueueTest PRIVATE -DTEST)

file(GLOB ManagerTest "./src/test/ManagerTest/*.cpp")
add_executable(ManagerTest ${ManagerTest} ${SRC})
target_link_libraries(ManagerTest PRIVATE gtest_main gmock_main)
add_test(NAME ManagerTest COMMAND ManagerTest)
gtest_discover_tests(ManagerTest)
target_compile_definitions(ManagerTest PRIVATE -DTEST)


