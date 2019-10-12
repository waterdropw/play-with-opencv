
cmake_minimum_required(VERSION 3.0)

project(play-with-opencv)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


option(BUILD_WITH_EXPERIMENT "Build with experiment tests" OFF)
option(BUILD_WITH_PERFORMANCE "Build with performance tests" OFF)


set(CMAKE_MACOSX_RPATH ON)


if (BUILD_WITH_EXPERIMENT)
    add_subdirectory(experiment)
endif()

if (BUILD_WITH_PERFORMANCE)
    add_subdirectory(performance)
endif()
