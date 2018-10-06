cmake_minimum_required(VERSION 3.12)
project(grok)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_library(libgit2 SHARED IMPORTED)
set_property(TARGET libgit2 PROPERTY IMPORTED_LOCATION "libgit2/build/libgit2.so")

include_directories(. libgit2/include)

add_executable(grok grok/main.cpp)
