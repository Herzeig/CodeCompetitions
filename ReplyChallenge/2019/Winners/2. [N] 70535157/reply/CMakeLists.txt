cmake_minimum_required(VERSION 3.10)
project(hashcode)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O0 -Wall -fsanitize=address,undefined -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -march=native -Wall -g0")

add_executable(score score.cpp)
add_executable(greedy greedy.cpp)
add_executable(flow_opt flow_opt.cpp)
add_executable(drochilovo drochilovo.cpp)
