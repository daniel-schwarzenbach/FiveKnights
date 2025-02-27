cmake_minimum_required(VERSION 3.20)

# Set C++23 as the standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# add SenpaiEngine
add_subdirectory(./Senpai)
# add Game
add_subdirectory(./Game)

# GTest #
# Include FetchContent module
include(FetchContent)

# Declare GoogleTest as a dependency
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

# Optionally set flags for GTest configuration
# For Windows compatibility
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  
# Download and make GoogleTest available
FetchContent_MakeAvailable(googletest)

# Add tests
enable_testing()

# Create a test executable (replace YOUR_TESTS with your actual test files)
add_executable(Tests ./tests/tests.cpp)

# Link GTest libraries
target_link_libraries(Tests PRIVATE gtest_main gtest Senpai)

# Discover and register tests
include(GoogleTest)
gtest_discover_tests(Tests)