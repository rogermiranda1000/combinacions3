# 3.17.3 MinGW; 3.10.2 WSL
cmake_minimum_required(VERSION 3.10)
project(combinacions3 C)

set(CMAKE_C_STANDARD 99)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

add_executable(combinacions3 main.c combination.c combination.h operaciones.c operaciones.h paralleling.c paralleling.h)

target_link_libraries(combinacions3 m Threads::Threads) # m: math library; Threads::Threads: pthread