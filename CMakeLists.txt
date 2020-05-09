
cmake_minimum_required(VERSION 3.0)

project(play-with-opencv)


if(ANDROID)
  add_compile_definitions(_ANDROID_PLATFORM_)
elseif(APPLE)
  add_compile_definitions(_OSX_PLATFORM_)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  add_compile_definitions(_LINUX_PLATFORM_)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
  add_compile_definitions(_WINDOWS_PLATFORM_)
endif()
if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    add_compile_definitions(_32_BIT_)
elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    add_compile_definitions(_64_BIT_)
endif()


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


option(BUILD_WITH_PERFTEST "Build with performance tests" ON)
option(BUILD_WITH_DEMO "Build with demo app" ON)

if (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif()

add_subdirectory(utils)

if (BUILD_WITH_PERFTEST)
    set(GOOGLETEST_PATH "${CMAKE_SOURCE_DIR}/external/googletest" CACHE INTERNAL "google test src path")
    add_subdirectory(external/benchmark)
    add_subdirectory(perftest)
endif()

if(BUILD_WITH_DEMO) 
    add_subdirectory(demo)
endif()


##############################################
# Installation instructions
include(GNUInstallDirs)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/media/ DESTINATION ${CMAKE_INSTALL_BINDIR}/media)
