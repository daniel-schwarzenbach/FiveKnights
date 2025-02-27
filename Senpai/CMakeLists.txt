# CMake File to Build the Senpai Engine
project(Senpai VERSION 0.0.1)

# SDL
   # target_link_directories(${PROJECT_NAME} 
   #    PUBLIC ${CMAKE_SOURCE_DIR}/library/SDL/lib)
   # target_include_directories(${PROJECT_NAME} 
   #    PUBLIC ${CMAKE_SOURCE_DIR}/library/SDL/include)
   # target_link_libraries(${PROJECT_NAME} SDL3)
   add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL)
   
   
#_

# get all cpp-files
file(GLOB_RECURSE SRC_FILES src/**.c++ CMAKE_CONFIGURE_DEPENDS)
add_library(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} 
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SDL/include)
target_link_libraries(${PROJECT_NAME} SDL3)


# Windows Build
if(WIN32)
    
endif(WIN32)

if(UNIX)

endif(UNIX)

# precompile headers
target_precompile_headers(${PROJECT_NAME} 
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/core/pch.h)

# include directory
target_include_directories(${PROJECT_NAME}
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


