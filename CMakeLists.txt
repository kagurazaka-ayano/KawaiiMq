cmake_minimum_required(VERSION 3.27)
project(KawaiiMQ)
include(CTest)
include(FetchContent)
set(CMAKE_CXX_STANDARD 23)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(GoogleTest)
enable_testing()
file(GLOB SRC "./src/*.cpp")
set(INCLUDE_USER "./include/${CMAKE_PROJECT_NAME}")
set(INCLUDE_THIRDPARTIES "dependencies/include")
set(LIB_ROOT "dependencies/lib")
set(INCLUDE ${INCLUDE_USER})

include_directories(${INCLUDE})
add_library(${CMAKE_PROJECT_NAME} STATIC ${SRC})

file(GLOB OperatorTest "./src/test/OperatorTest/*.cpp")
add_executable(OperatorTest ${OperatorTest} ${SRC})
target_link_libraries(OperatorTest PRIVATE GTest::GTest)
add_test(NAME OperatorTest COMMAND OperatorTest)
gtest_discover_tests(OperatorTest)

file(GLOB QueueTest "./src/test/QueueTest/*.cpp")
add_executable(QueueTest ${QueueTest} ${SRC})
target_link_libraries(QueueTest PRIVATE GTest::GTest)
add_test(NAME QueueTest COMMAND QueueTest)
gtest_discover_tests(QueueTest)





