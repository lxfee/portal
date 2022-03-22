# cmake version
cmake_minimum_required(VERSION 3.1)

add_compile_options(-O3)

# vcpkg

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")

# project
project(main)

# target
aux_source_directory("./" PROJECT_SOURCES)

add_executable(main ${PROJECT_SOURCES})
target_include_directories(main PRIVATE include)
include_directories(include)
include_directories(${ASSIMP_INCLUDE_DIRS})





# dependency
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(assimp REQUIRED)
find_package(glm CONFIG REQUIRED)

# copy
file(INSTALL "$ENV{ASSIMP_DIR}/bin/libassimp-5.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


# link

target_link_libraries (main PRIVATE ${ASSIMP_LIBRARIES})
target_link_libraries(main PRIVATE GLUT::GLUT)
target_link_libraries(main PRIVATE GLEW::GLEW)
target_link_libraries(main PRIVATE glm::glm)


