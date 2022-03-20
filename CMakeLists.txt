# cmake version
cmake_minimum_required(VERSION 3.1)

# c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-O3)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# project
project(main)

# target
aux_source_directory("./" PROJECT_SOURCES)

add_executable(main ${PROJECT_SOURCES})
target_include_directories(main PRIVATE include)

# dependency
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
# link
target_link_libraries(main PRIVATE GLUT::GLUT)
target_link_libraries(main PRIVATE GLEW::GLEW)
