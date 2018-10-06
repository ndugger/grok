cmake_minimum_required(VERSION 3.12)
project(grok)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

include_directories(.)

add_executable(grok grok/main.cpp)
