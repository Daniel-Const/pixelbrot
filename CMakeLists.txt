cmake_minimum_required (VERSION 3.2)
project (pixelbrot)

# SDL Package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})
add_definitions(${SDL2_DEFINITIONS})
if(NOT SDL2_FOUND)
    message(ERROR " SDL2 not found!")
endif(NOT SDL2_FOUND)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${oPENgl_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if (NOT OPENGL_FOUND)
    message(ERROR. " OPENGL not found")
endif(NOT OPENGL_FOUND)

include_directories(${SDL2_INCLUDE_DIRS}, ${OPENGL_INCLUDE_DIRS})

include_directories(lib/imgui)

# Project files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# Imgui files
file(GLOB_RECURSE IMGUI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/*.cpp)

add_executable(pixelbrot ${SOURCES} ${IMGUI_SOURCES})

target_link_libraries(pixelbrot ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})
