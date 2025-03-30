# CMake File to Build the Senpai Engine
project(Senpai VERSION 0.0.1)

if(WIN32)
   # Use or do not use vendored libs
   set(SDLTTF_VENDORED ON)
   set(SDLIMAGE_VENDORED OFF)
   set(SDLMIXER_VENDORED OFF)
else()
   set(SDLTTF_VENDORED OFF)
   set(SDLIMAGE_VENDORED OFF)
   set(SDLMIXER_VENDORED OFF)
endif()



# Add SDL subsystems first
add_subdirectory(lib/SDL)
add_subdirectory(lib/SDL_ttf)
add_subdirectory(lib/SDL_image)
add_subdirectory(lib/SDL_mixer)





# get all cpp-files
file(GLOB_RECURSE SRC_FILES src/**.c++ CMAKE_CONFIGURE_DEPENDS)


if(WIN32)
    # Build as static library on Windows
    add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
else()
    add_library(${PROJECT_NAME} ${SRC_FILES})
endif()

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


# precompile headers
target_precompile_headers(${PROJECT_NAME} 
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/core/Core.h++)

# include directory
target_include_directories(${PROJECT_NAME}
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


