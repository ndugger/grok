cmake_minimum_required(VERSION 3.12)
project(grok)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(grok grok/main.cpp)

include_directories(. libgit2/include)

add_library(libgit2 SHARED IMPORTED)
set_target_properties(libgit2 PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libgit2/build/libgit2.so")

target_link_libraries(grok libgit2)
