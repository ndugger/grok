cmake_minimum_required(VERSION 3.12)
project(grok)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

include_directories(.)
add_executable(grok grok/main.cpp)

include_directories(libgit2/include)
add_library(git2 SHARED IMPORTED)
set_target_properties(git2 PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libgit2/build/libgit2.so")

include_directories(json/single_include)

target_link_libraries(grok git2 stdc++fs)
