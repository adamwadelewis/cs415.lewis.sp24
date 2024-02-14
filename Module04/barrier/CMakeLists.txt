cmake_minimum_required(VERSION 2.9)

#set the project name
project(barriers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add the executable
add_executable(stlbarriers stlbarriers.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(stlbarriers PRIVATE Threads::Threads)

