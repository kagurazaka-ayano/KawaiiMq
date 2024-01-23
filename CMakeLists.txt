cmake_minimum_required(VERSION 3.27)
project(messageQueue)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SRC "./src/*.cpp")
set(INCLUDE_USER "./include/${CMAKE_PROJECT_NAME}")
set(INCLUDE_THIRDPARTIES "dependencies/include")
set(LIB_ROOT "dependencies/lib/")
file(GLOB LIB_GTEST "${LIB_ROOT}/gtest/${CMAKE_SYSTEM_NAME}/*.a")

list(APPEND LIB_LIST ${LIB_GTEST})

set(INCLUDE ${INCLUDE_USER} ${INCLUDE_THIRDPARTIES})
set(LIBS ${LIB_GTEST})

add_library(${CMAKE_PROJECT_NAME} STATIC ${SRC})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${INCLUDE})

target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${LIBS})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${LIB_LIST})


