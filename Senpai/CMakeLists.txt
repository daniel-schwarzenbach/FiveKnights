# CMake File to Build the Senpai Engine
project(Senpai VERSION 0.0.1)


# Add SDL subsystems first
add_subdirectory(lib/SDL)
add_subdirectory(lib/SDL_ttf)
add_subdirectory(lib/SDL_image)
add_subdirectory(lib/SDL_mixer)



# get all cpp-files
file(GLOB_RECURSE SRC_FILES src/**.c++ CMAKE_CONFIGURE_DEPENDS)
add_library(${PROJECT_NAME} ${SRC_FILES})

# prioritize the SDL3 headers
target_compile_options(${PROJECT_NAME} PRIVATE
    -I${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL/include
    -I${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL_image/include)

# Link SDL3 libraries using CMake targets
target_link_libraries(${PROJECT_NAME}
    PRIVATE 
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
)


# Windows Build
if(WIN32)
    
endif(WIN32)

if(UNIX)

endif(UNIX)

# precompile headers
target_precompile_headers(${PROJECT_NAME} 
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/core/Core.h++)

# include directory
target_include_directories(${PROJECT_NAME}
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


