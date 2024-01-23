cmake_minimum_required(VERSION 3.27)
project(messageQueue)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SRC "./src/*.cpp")
set(INCLUDE_USER "./include/${CMAKE_PROJECT_NAME}")
set(INCLUDE ${INCLUDE_USER})

add_library(${CMAKE_PROJECT_NAME} STATIC ${SRC})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${INCLUDE})



