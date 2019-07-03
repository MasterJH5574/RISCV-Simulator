cmake_minimum_required(VERSION 3.5.1)

project(PPCA_risc-v)

set(CMAKE_CXX_STANDARD 11)

add_executable(code main.cpp instructions.cpp global.cpp stages.cpp)