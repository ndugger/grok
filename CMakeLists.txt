cmake_minimum_required(VERSION 3.12)
project(grok)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

include_directories(.)

add_executable(
        grok
        grok/main.cpp
        grok/objects/command.cpp
        grok/objects/configuration.cpp
        grok/objects/package.cpp
        grok/commands/help.cpp
        grok/commands/new.cpp
        grok/commands/sync.cpp
        grok/commands/update.cpp
        grok/commands/use.cpp)
