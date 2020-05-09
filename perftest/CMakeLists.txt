
cmake_minimum_required(VERSION 3.0)

project(cv_perf VERSION 0.1.0 LANGUAGES CXX)


find_package(OpenCV 4 REQUIRED)


add_executable(tapi tapi.cpp)
target_link_libraries(tapi
    PRIVATE 
        ${OpenCV_LIBS}
        xbwee::utils
)

target_compile_features(tapi PUBLIC cxx_std_11)
target_compile_options(tapi PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

add_executable(memcpy memcpy.cpp)
target_link_libraries(memcpy
    PRIVATE 
        xbwee::utils
        benchmark::benchmark
        benchmark::benchmark_main
)
target_compile_features(memcpy PUBLIC cxx_std_17)
target_compile_options(memcpy PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)
# target_compile_options(memcpy PRIVATE -O1)



add_executable(oprtest oprtest.cpp Profiler.cpp)
target_link_libraries(oprtest
    PRIVATE 
        ${OpenCV_LIBS}
        xbwee::utils
        benchmark::benchmark
        benchmark::benchmark_main
)
target_compile_features(oprtest PUBLIC cxx_std_17)
target_compile_options(oprtest PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)


add_executable(perftest perftest.cpp Profiler.cpp)
target_link_libraries(perftest
    PRIVATE 
        ${OpenCV_LIBS}
        xbwee::utils
        benchmark::benchmark
        benchmark::benchmark_main
)
target_compile_features(perftest PUBLIC cxx_std_17)
target_compile_options(perftest PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)


add_executable(memeater memeater.c)
add_executable(pftest pftest.c)

if(ANDROID OR CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_executable(pmudemo perf_event_open.c)
    target_include_directories(pmudemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if (ANDROID AND CMAKE_ANDROID_ARCH EQUAL "arm")
    enable_language(C ASM)
    add_executable(icache
        icachetest/icache_main.cpp 
        icachetest/icache.S
        Profiler.cpp
    )
    set_source_files_properties(icachetest/icache.S PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")
    target_include_directories(icache PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

##############################################
# Installation instructions
include(GNUInstallDirs)

install(TARGETS tapi
    EXPORT play-with-opencv-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # for Windows
)

install(TARGETS memcpy
    EXPORT play-with-opencv-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # for Windows
)


install(TARGETS oprtest
    EXPORT play-with-opencv-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # for Windows
)

install(TARGETS perftest
    EXPORT play-with-opencv-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # for Windows
)


install(TARGETS memeater
    EXPORT play-with-opencv-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # for Windows
)

install(TARGETS pftest
    EXPORT play-with-opencv-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # for Windows
)
if(ANDROID OR CMAKE_SYSTEM_NAME MATCHES "Linux")
install(TARGETS pmudemo
    EXPORT play-with-opencv-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # for Windows
)
endif()

if (CMAKE_ANDROID_ARCH EQUAL "arm")
install(TARGETS icache
    EXPORT play-with-opencv-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # for Windows
)
endif()
