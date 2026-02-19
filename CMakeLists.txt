cmake_minimum_required(VERSION 3.16)
project(lru_cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(lru_cache INTERFACE)
target_include_directories(lru_cache INTERFACE include)

add_executable(lru_cache_demo src/main.cpp)
target_link_libraries(lru_cache_demo PRIVATE lru_cache)

add_executable(lru_cache_tests src/tests.cpp)
target_link_libraries(lru_cache_tests PRIVATE lru_cache)
