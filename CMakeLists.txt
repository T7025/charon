cmake_minimum_required(VERSION 3.9)
project(charon)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")  # enable OpenMP

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Og")


add_executable(charon main.cpp Universe.cpp Universe.h Body.cpp Body.h Tree.cpp Tree.h Position.cpp Position.h Velocity.cpp Velocity.h)