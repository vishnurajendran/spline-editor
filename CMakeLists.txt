cmake_minimum_required(VERSION 3.16)
project(spline_editor)

set(CMAKE_CXX_STANDARD 20)
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Libigl
include(libigl)
igl_include(glfw)
igl_include(imgui)
igl_include(opengl)

# Add your project files
add_executable(${PROJECT_NAME}
        src/typedefines.h
        src/controlmesh.h
        src/controlmesh.cpp
        src/bsplinemesh.h
        src/bsplinemesh.cpp
        src/splinecore.h
        src/main.cpp
        src/editor.cpp
        src/editor.h
)

# Target-specific include directories
target_include_directories(${PROJECT_NAME} PUBLIC
        ${LIBIGL_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/src
)

# Link igl modules
target_link_libraries(${PROJECT_NAME} PUBLIC
        igl::glfw
        igl::imgui
        igl::opengl
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
