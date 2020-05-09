cmake_minimum_required(VERSION 3.0)

project(ocv_demo VERSION 0.1.0 LANGUAGES CXX)


find_package(OpenCV 4 REQUIRED)


add_executable(capture_edge capture_edge.cpp)
target_link_libraries(capture_edge
    PRIVATE
        ${OpenCV_LIBS}
)

add_executable(lkdemo lkdemo.cpp)
target_link_libraries(lkdemo PRIVATE ${OpenCV_LIBS})

add_executable(segment_gray anisotropic_image_segmentation.cpp)
target_link_libraries(segment_gray
    PRIVATE 
        ${OpenCV_LIBS}
        xbwee::utils
)



##############################################
# Installation instructions
include(GNUInstallDirs)

install(TARGETS capture_edge
    EXPORT play-with-opencv-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # for Windows
)
install(TARGETS lkdemo
    EXPORT play-with-opencv-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # for Windows
)
install(TARGETS segment_gray
    EXPORT play-with-opencv-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # for Windows
)