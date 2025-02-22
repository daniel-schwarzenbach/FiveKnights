cmake_minimum_required(VERSION 3.20)
# CMake File to Build the Senpai Engine
project(Senpai VERSION 0.0.1)

# SDL
    # set the output directory for built objects.
    # This makes sure that the dynamic library goes into the build directory automatically.
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 
    "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY 
    "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
    # This assumes the SDL source is available in vendored/SDL
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL EXCLUDE_FROM_ALL)
#_

# get all cpp-files
file(GLOB_RECURSE SRC_FILES src/*.cpp CMAKE_CONFIGURE_DEPENDS)(
add_library(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

set_target_properties(MyLibrary PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../lib"    # For static libraries (.lib/.a)
    LIBRARY_OUTPUT_DIRECTORY "../lib"    # For shared libraries (.so/.dylib)
    #RUNTIME_OUTPUT_DIRECTORY "../bin"    # For executables and DLLs on Windows
)



# Windows Build
if(WIN32)
    
endif(WIN32)

if(UNIX)

endif(UNIX)

# precompile headers
target_precompile_headers(${PROJECT_NAME} 
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

# include directory
target_precompile_headers(${PROJECT_NAME} 
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
