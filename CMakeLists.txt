# cmake version
cmake_minimum_required(VERSION 3.1)

add_compile_options(-O3)

# vcpkg
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()
if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

# set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
# set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")

# project
project(main)

# target
aux_source_directory("./" PROJECT_SOURCES)

add_executable(main ${PROJECT_SOURCES})
target_include_directories(main PRIVATE include)
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


