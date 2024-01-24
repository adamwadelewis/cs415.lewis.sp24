cmake_minimum_required(VERSION 3.8)

#set the project name
project(forkexamples)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add the executable
add_executable(fork_ex fork_ex.cpp)

add_executable(wait_ex wait_ex.cpp)

add_executable(waitorkill waitorkill.cpp)

add_executable(exec_ex exec_ex.cpp)

add_executable(dup_ex dup_ex.cpp)



