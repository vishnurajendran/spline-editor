cmake_minimum_required(VERSION 3.16)
project(spline_editor)

set(CMAKE_CXX_STANDARD 20)

find_package(Python3 REQUIRED)
include(FetchContent)

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG master
)

FetchContent_MakeAvailable(glad)

add_executable(spline_editor main.cpp)

target_link_libraries(spline_editor PRIVATE glad)

target_include_directories(spline_editor PRIVATE
        ${glad_SOURCE_DIR}/include  # Correct way to include glad
        ${CMAKE_SOURCE_DIR}/vendor/libigl/include
        ${CMAKE_SOURCE_DIR}/vendor/eigen_src
)
