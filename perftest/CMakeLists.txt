
cmake_minimum_required(VERSION 3.0)

project(cv_perf VERSION 0.1.0 LANGUAGES CXX)


find_package(OpenCV 4 REQUIRED)


add_executable(tapi tapi.cpp)
target_link_libraries(tapi
    PRIVATE 
        ${OpenCV_LIBS}
        COMMON::utils
)

target_compile_features(tapi PUBLIC cxx_std_11)
target_compile_options(tapi PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

add_executable(memcpy memcpy.cpp)
target_link_libraries(memcpy
    PRIVATE 
        COMMON::utils
        benchmark::benchmark
        benchmark::benchmark_main
)
target_compile_features(memcpy PUBLIC cxx_std_17)
target_compile_options(memcpy PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)
# target_compile_options(memcpy PRIVATE -O1)



add_executable(oprtest oprtest.cpp)
target_link_libraries(oprtest
    PRIVATE 
        ${OpenCV_LIBS}
        COMMON::utils
        benchmark::benchmark
        benchmark::benchmark_main
)
target_compile_features(oprtest PUBLIC cxx_std_17)
target_compile_options(oprtest PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

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