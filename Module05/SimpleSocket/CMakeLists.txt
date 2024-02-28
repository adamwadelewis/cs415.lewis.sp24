cmake_minimum_required(VERSION 2.9)

#set the project name
project(socketdemo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add the executable
add_executable(simple_server simple_server_main.cpp ServerSocket.cpp Socket.cpp)
add_executable(simple_client simple_client_main.cpp ClientSocket.cpp Socket.cpp)

