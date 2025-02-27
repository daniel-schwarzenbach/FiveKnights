project(FiveKnights VERSION 0.0.1)

file(GLOB_RECURSE SRC_FILES src/*.c++ CMAKE_CONFIGURE_DEPENDS)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} Senpai)

include_directories(${CMAKE_SOURCE_DIR}/library/Senpai/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)