cmake_minimum_required(VERSION 3.20)
project(FiveKnights)

# Set C++23 as the standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Output
set(EXECUTABLE_OUTPUT_PATH ../)

# add SenpaiEngine
add_subdirectory(./library/Senpai)
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
    add_executable(Test.out ./tests/test.cpp)

    # Link GTest libraries
    target_link_libraries(Test.out PRIVATE gtest_main gtest)

    # Add the test executable to the testing suite
    add_test(NAME my_tests COMMAND my_tests)
# end Gtest

# # Test #
# find_package(GTest REQUIRED)

# add_executable(Test.out ./tests/test.cpp)
# target_link_libraries(Test.out GTest::gtest_main) # Link with GTest and your library

# include(GoogleTest)
# gtest_discover_tests(Test.out) # Automatically discover tests