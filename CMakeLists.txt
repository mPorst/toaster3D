cmake_minimum_required(VERSION 3.20)

project(renderEngine)
set(thisApp "${PROJECT_NAME}_test")

find_package(OpenGL REQUIRED)

add_subdirectory(OpenGL)
set(FT_DISABLE_ZLIB TRUE)
set(FT_DISABLE_BZIP2 TRUE)
set(FT_DISABLE_PNG TRUE)
set(FT_DISABLE_HARFBUZZ TRUE)
set(FT_DISABLE_BROTLI TRUE)
add_subdirectory(external/freetype)
add_subdirectory(external/glfw)

add_executable(${thisApp} main.cpp)

target_link_libraries(${thisApp} OpenGLRenderer glfw glew OpenGL::GL freetyped)
target_compile_features(${thisApp} PRIVATE cxx_std_20)


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)
add_custom_command(TARGET ${thisApp} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets)