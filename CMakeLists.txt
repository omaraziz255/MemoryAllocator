cmake_minimum_required(VERSION 3.24)
project(MemoryAllocator C)

set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(MemoryAllocatorExample main.c)
target_link_libraries(MemoryAllocatorExample MemoryAllocator)
