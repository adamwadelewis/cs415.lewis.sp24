cmake_minimum_required(VERSION 2.9)

#set the project name
project(devsim)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add the executable
add_executable(futures futures.cpp)




set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(futures PRIVATE Threads::Threads)

