cmake_minimum_required(VERSION 3.20)
project(SenpaiTests)

# Set C++23 as the standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# file(GLOB sourceFiles
#   "src/*.cpp"
# )
#add_library(Utility ${sourceFiles})

set(EXECUTABLE_OUTPUT_PATH ../)

# Test #
find_package(GTest REQUIRED)

add_executable(App.out ./test.cpp)
target_link_libraries(App.out GTest::gtest_main Senpai) # Link with GTest and your library

include(GoogleTest)
gtest_discover_tests(App.out) # Automatically discover tests