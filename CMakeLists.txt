cmake_minimum_required(VERSION 3.21)
project(kvcache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(kvcache
        main.cpp

        infrastructure/periodic/PeriodicTask.cpp
        application/feature/protocol/Http.cpp
)

include_directories(${CMAKE_SOURCE_DIR})


target_compile_definitions(kvcache PRIVATE PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")


# Libs
target_include_directories(kvcache
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)
