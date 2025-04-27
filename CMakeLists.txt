cmake_minimum_required(VERSION 3.28)

project(N-Body_Sim)

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)

include_directories({PYTHIN_INCLUDE_DIRS} include)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

set(SFML_USE_DRM false)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(NBSIM_DrawTree src/main.cpp src/Body.cpp src/Force.cpp src/Simulation.cpp src/QuadTree.cpp)

target_link_libraries(NBSIM_DrawTree Python3::NumPy SFML::Graphics)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})


