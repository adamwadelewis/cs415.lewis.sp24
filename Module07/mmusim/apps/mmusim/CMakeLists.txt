cmake_minimum_required(VERSION 3.11)

#set the project name
project(mmusim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(../../include
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/source)

file(GLOB all_SRCS
  "../../include/*.h"
  "../../include/*.hpp"
  "${PROJECT_SOURCE_DIR}/include/*.h"
  "${PROJECT_SOURCE_DIR}/include/*.hpp"
  "${PROJECT_SOURCE_DIR}/source/*.cpp"
  "${PROJECT_SOURCE_DIR}/*.cpp"
  )

# Do the required setup for CMake
# Get the stuff we need to use Google Test...
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#add the executable
add_executable(mmusimtest ${all_SRCS})
target_link_libraries(mmusimtest gtest_main)

include(GoogleTest)
gtest_discover_tests(mmusimtest)



