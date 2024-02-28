cmake_minimum_required(VERSION 2.9)

#set the project name
project(sharedmemdemo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add the executable
add_executable(shm_server shm_server.c)
add_executable(shm_client shm_client.c)

add_executable(mmapdemo mmapdemo.c)
