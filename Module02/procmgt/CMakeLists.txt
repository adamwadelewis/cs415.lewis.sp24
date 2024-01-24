cmake_minimum_required(VERSION 3.4)

#set the project name
project(procmgt)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add the executable
add_executable(getpid getpid.cpp)

add_executable(dataloc dataloc.cpp)

