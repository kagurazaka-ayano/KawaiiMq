cmake_minimum_required(VERSION 3.27)
project(KawaiiMQ)
include(CTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SRC "./src/*.cpp")
file(GLOB SRC_TEST "./src/test/*.cpp")
set(INCLUDE_USER "./include/${CMAKE_PROJECT_NAME}")
set(INCLUDE_THIRDPARTIES "dependencies/include")
set(LIB_ROOT "dependencies/lib")
file(GLOB LIB_GTEST "${LIB_ROOT}/gtest/${CMAKE_SYSTEM_NAME}/*.a")

list(APPEND LIB_LIST ${LIB_GTEST})

set(INCLUDE ${INCLUDE_USER} ${INCLUDE_THIRDPARTIES})

include_directories(${INCLUDE})
add_library(${CMAKE_PROJECT_NAME} STATIC ${SRC})

add_executable(${CMAKE_PROJECT_NAME}_TEST ${SRC_TEST} ${SRC})
target_link_libraries(${CMAKE_PROJECT_NAME}_TEST PUBLIC ${LIB_GTEST})


