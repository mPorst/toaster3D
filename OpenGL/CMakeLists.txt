cmake_minimum_required(VERSION 3.14)

#project(OpenGLRenderer)

set(CMAKE_CXX_STANDARD 20)

set(renderSrc 
  OpenGLRenderer.cpp 
  TriangleEntity.cpp 
  RectangleEntity.cpp
  SphereEntity.cpp 
  Camera.cpp 
  TextureLoading/loadBmp.cpp
)

set(renderHdr 
  OpenGLRenderer.h 
  TriangleEntity.h 
  RectangleEntity.hpp
  SphereEntity.hpp 
  Camera.h 
  TextureLoading/loadBmp.hpp
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glew)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../shaders)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/SimpleFragmentShader.fragmentshader
    ${CMAKE_CURRENT_BINARY_DIR}/../shaders COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/SimpleVertexShader.vertexshader
    ${CMAKE_CURRENT_BINARY_DIR}/../shaders COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/TextFragmentShader.fragmentshader
    ${CMAKE_CURRENT_BINARY_DIR}/../shaders COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/TextVertexShader.vertexshader
    ${CMAKE_CURRENT_BINARY_DIR}/../shaders COPYONLY)

find_package(OpenGL REQUIRED)

add_library(OpenGLRenderer ${renderSrc} ${renderSrc})

target_link_directories(OpenGLRenderer PUBLIC ${PROJECT_BINARY_DIR}/external/freetype/Debug)

message(${PROJECT_SOURCE_DIR})
target_include_directories(OpenGLRenderer PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(OpenGLRenderer PRIVATE ${PROJECT_SOURCE_DIR}/OpenGL/glew/include)
target_include_directories(OpenGLRenderer PRIVATE ${PROJECT_SOURCE_DIR}/external/glfw/include)
target_include_directories(OpenGLRenderer PUBLIC ${PROJECT_SOURCE_DIR}/external/freetype/include)

