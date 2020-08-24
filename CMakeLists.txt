cmake_minimum_required(VERSION 3.10)
project(combinacions3 C)

set(CMAKE_C_STANDARD 99)

add_executable(combinacions3 main.c combination.c combination.h)

target_link_libraries(combinacions3 m)