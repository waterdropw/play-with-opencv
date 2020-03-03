
cmake_minimum_required(VERSION 3.0)

project(play-with-opencv)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


option(BUILD_WITH_EXPERIMENT "Build with experiment tests" ON)
option(BUILD_WITH_PERFORMANCE "Build with performance tests" ON)


set(CMAKE_MACOSX_RPATH ON)

add_subdirectory(utils)

if (BUILD_WITH_EXPERIMENT)
    add_subdirectory(experiment)
endif()

if (BUILD_WITH_PERFORMANCE)
    add_subdirectory(performance)
endif()


##############################################
# Installation instructions
include(GNUInstallDirs)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/media/ DESTINATION ${CMAKE_INSTALL_BINDIR}/media)
