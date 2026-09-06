cmake_minimum_required(VERSION 3.21)
project(kvcache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(kvcache
        main.cpp

        infrastructure/hashtable/HashTable.hpp
        infrastructure/di/DIContainer.hpp
        infrastructure/config/env.hpp
        infrastructure/periodic/PeriodicTask.cpp
        infrastructure/periodic/PeriodicTask.hpp

        application/feature/protocol/http.cpp
        application/feature/protocol/http.hpp

        domain/abstracts/IHashTable.hpp
        domain/abstracts/IServer.hpp
        domain/abstracts/type.hpp
        domain/entity/Value.hpp
)

include_directories(${CMAKE_SOURCE_DIR})


target_compile_definitions(kvcache PRIVATE PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")


# Libs
target_include_directories(kvcache
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)
