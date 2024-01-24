cmake_minimum_required(VERSION 2.4)

#set the project name
project(matmult)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add the executable
add_executable(matmult matmult.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(matmult PRIVATE Threads::Threads)


