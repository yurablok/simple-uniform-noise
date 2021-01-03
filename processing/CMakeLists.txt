cmake_minimum_required(VERSION 3.12)
include("CMakeUtils.cmake")

check_build_directory()

project("simple-uniform-noise")

init_project("${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS FALSE)
add_subdirectory("deps/SFML" SFML)

add_executable(${PROJECT_NAME}
    "../noise.hpp"
    "../polyfit.hpp"
    "../staff.hpp"

    "main.cpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
)
