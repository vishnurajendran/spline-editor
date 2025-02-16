cmake_minimum_required(VERSION 3.16)
project(spline_editor)

set(CMAKE_CXX_STANDARD 20)

# Find Python3
find_package(Python3 REQUIRED)

# Include FetchContent for dependency management
include(FetchContent)

# Fetch glad for OpenGL function loading
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG master
)
FetchContent_MakeAvailable(glad)

# Find GLFW installed via Homebrew
find_package(glfw3 REQUIRED)

# Add executable
add_executable(spline_editor main.cpp)

# Link glad and glfw
target_link_libraries(spline_editor PRIVATE glad glfw)

# Include directories
target_include_directories(spline_editor PRIVATE
        #${glad_SOURCE_DIR}/include  # Glad include path
        ${CMAKE_SOURCE_DIR}/vendor/libigl/include
        ${CMAKE_SOURCE_DIR}/vendor/eigen_src
        /opt/homebrew/include  # For GLFW (Apple Silicon)
        /usr/local/include      # For GLFW (Intel Macs)
)

# Ensure GLFW is linked from the correct path
target_link_directories(spline_editor PRIVATE
        /opt/homebrew/lib  # Apple Silicon
        /usr/local/lib      # Intel Macs
)

set_target_properties(spline_editor PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)