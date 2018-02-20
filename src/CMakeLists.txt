cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")  # enable OpenMP

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Og")

set(core_SRCS Universe.cpp Universe.h Body.cpp Body.h Tree.cpp Tree.h Position.cpp Position.h Velocity.cpp Velocity.h Vector2.cpp Vector2.h)

add_library(core ${core_SRCS})
add_executable(charon main.cpp)
target_link_libraries(charon core)