# cmake version
cmake_minimum_required(VERSION 3.1)

# vcpkg

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")

# project
project(Portal)

# target
aux_source_directory("./" PROJECT_SOURCES)
add_executable(main ${PROJECT_SOURCES})
include_directories(include)

# dependency
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# link

target_link_libraries(main PRIVATE assimp::assimp)
target_link_libraries(main PRIVATE GLUT::GLUT)
target_link_libraries(main PRIVATE GLEW::GLEW)
target_link_libraries(main PRIVATE glm::glm)


