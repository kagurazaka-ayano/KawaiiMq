cmake_minimum_required(VERSION 3.27)
project(messageQueue)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SRC "./src/*.cpp")
file(GLOB_RECURSE SRC "./src/UnitTest/*.cpp")
set(INCLUDE_USER "./include/${CMAKE_PROJECT_NAME}")
set(INCLUDE_THIRDPARTIES "dependencies/include")
set(LIB_ROOT "dependencies/lib/")
file(GLOB LIB_GTEST "${LIB_ROOT}/gtest/${CMAKE_SYSTEM_NAME}/*.a")

list(APPEND LIB_LIST ${LIB_GTEST})

set(INCLUDE ${INCLUDE_USER} ${INCLUDE_THIRDPARTIES})
set(LIBS ${LIB_GTEST})

include_directories(${INCLUDE})
link_directories(${LIBS})
link_libraries(${LIB_LIST})

add_library(${CMAKE_PROJECT_NAME} STATIC ${SRC})
add_executable(${CMAKE_PROJECT_NAME}_QUEUE_TEST ${SRC})


