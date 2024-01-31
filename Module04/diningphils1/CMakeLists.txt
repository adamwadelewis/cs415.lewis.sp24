cmake_minimum_required(VERSION 2.9)

#set the project name
project(diningphils)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add the executable
add_executable(diningphil diningphil.cpp)
add_executable(diningphils2 diningphils2.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(diningphil PRIVATE Threads::Threads)
target_link_libraries(diningphils2 PRIVATE Threads::Threads)
