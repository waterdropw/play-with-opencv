
cmake_minimum_required(VERSION 3.0)

project(play-with-opencv)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


option(BUILD_WITH_PERFTEST "Build with performance tests" ON)


set(CMAKE_MACOSX_RPATH ON)

add_subdirectory(utils)

if (BUILD_WITH_PERFTEST)
    set(GOOGLETEST_PATH "${CMAKE_SOURCE_DIR}/external/googletest" CACHE INTERNAL "google test src path")
    add_subdirectory(external/benchmark)
    add_subdirectory(perftest)
endif()


##############################################
# Installation instructions
include(GNUInstallDirs)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/media/ DESTINATION ${CMAKE_INSTALL_BINDIR}/media)
