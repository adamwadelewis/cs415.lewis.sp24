cmake_minimum_required(VERSION 2.4)

#set the project name
project(matmult)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add the executable
add_executable(chrono chrono.cpp)
add_executable(hello hello.cpp)
add_executable(hellolambda hellolambda.cpp)
add_executable(race race.cpp)
add_executable(thvector thvector.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(chrono PRIVATE Threads::Threads)
target_link_libraries(hello PRIVATE Threads::Threads)
target_link_libraries(hellolambda PRIVATE Threads::Threads)
target_link_libraries(thvector PRIVATE Threads::Threads)
target_link_libraries(race PRIVATE Threads::Threads)

