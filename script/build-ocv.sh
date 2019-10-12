#!/bin/bash

HOST=`uname -sm`
TARGET_OS=$1
HOST_SYSTEM=`uname`
CPU_CORES=4
if [[ -z "${TARGET_OS}" ]]; then
    TARGET_OS=${HOST_SYSTEM}
fi
if [[ ${TARGET_OS} == "Android" || ${TARGET_OS} == "android" ]]; then
    TARGET_OS="Android"
fi

if [[ ${HOST_SYSTEM} == "Linux" ]]; then
    CPU_CORES=`cat /proc/cpuinfo |grep "processor"|wc -l`
elif [[ ${HOST_SYSTEM} == "Darwin" ]]; then
    CPU_CORES=`sysctl -n hw.physicalcpu`
fi

BUILD_ROOT=out/opencv/build/${TARGET_OS}
INSTALL_ROOT=out/opencv/distrib/${TARGET_OS}

SRC_DIR=external/opencv
EXTRA_MODULE_PATH=external/opencv_contrib/modules

echo "*** *** *** *** *** *** *** *** *** *** *** ***"
echo "HOST_OS: ${HOST}"
echo "TARGET_OS: ${TARGET_OS}"
echo "CPU_CORES: ${CPU_CORES}"
echo "SRC Dir: ${SRC_DIR}"
echo "Build Dir: ${BUILD_ROOT}"
echo "Install Dir: ${INSTALL_ROOT}"
echo "*** *** *** *** *** *** *** *** *** *** *** ***"

if [[ ${TARGET_OS} == "Android" ]]; then
    if [[ -d "$ANDROID_NDK_ROOT" ]]; then
        ANDROID_NDK=$ANDROID_NDK_ROOT
    elif [[ -d "$ANDROID_HOME" ]]; then
        ANDROID_NDK=$ANDROID_HOME\ndk-bundle
    fi
    if [[ -d "$ANDROID_HOME" ]]; then
        ANDROID_SDK=$ANDROID_HOME
    fi
    if [[ ! -e "${ANDROID_NDK}" ]]; then
        echo "Error: ANDROID_NDK is not fuond"
        exit
    fi
    if [[ ! -e "${ANDROID_SDK}" ]]; then
        echo "Error: ANDROID_SDK is not found"
        exit
    fi
fi

CMAKE_CONFIG="-G "Ninja" \\
    -D CMAKE_INSTALL_PREFIX=${INSTALL_DIR} \\
    -D CMAKE_POSITION_INDEPENDENT_CODE=ON \\
    -D OPENCV_EXTRA_MODULES_PATH=${EXTRA_MODULE_PATH} \\
    -D ENABLE_CXX11=ON \\
    -D BUILD_SHARED_LIBS=ON \\
    -D BUILD_opencv_world=OFF \\
    -D BUILD_TESTS=OFF \\
    -D BUILD_EXAMPLES=ON \\
    -D BUILD_JAVA=ON \\
    -D BUILD_opencv_python2=OFF \\
    -D BUILD_opencv_python_bindings_generator=OFF \\
    -D BUILD_opencv_python_tests=OFF \\
    -D BUILD_opencv_java=ON \\
    -D BUILD_opencv_java_bindings_generator=ON \\
    -D WITH_OPENCL=ON \\
    -D WITH_OPENMP=ON \\
    -D WITH_HALIDE=ON \\
    -D WITH_TBB=ON "


if [[ ${TARGET_OS} == "Android" ]]; then
    CMAKE_CONFIG="${CMAKE_CONFIG} \\
    -D BUILD_ANDROID_PROJECTS=OFF \\
    -D BUILD_opencv_freetype=OFF \\
    -D CMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \\
    -D ANDROID_SDK=${ANDROID_SDK} \\
    -D ANDROID_NDK=${ANDROID_NDK} \\
    -D ANDROID_PLATFORM=android-28 \\
    -D ANDROID_STL=c++_static \\
    -D ANDROID_PIE=ON "
fi


if [[ ${TARGET_OS} == "Android" ]]; then
    BUILD_DIR=${BUILD_ROOT}/arm
    INSTALL_DIR=${INSTALL_ROOT}/arm
    cmake ${CMAKE_CONFIG} -D ANDROID_ABI="armeabi-v7a with NEON" -D CMAKE_INSTALL_PREFIX=${INSTALL_DIR} -B ${BUILD_DIR} ${SRC_DIR}
    cmake --build ${BUILD_DIR} --config Release --target install -- -j${CPU_CORES}

    BUILD_DIR=${BUILD_ROOT}/aarch64
    INSTALL_DIR=${INSTALL_ROOT}/aarch64
    cmake ${CMAKE_CONFIG} -D ANDROID_ABI="arm64-v8a" -D CMAKE_INSTALL_PREFIX=${INSTALL_DIR} -B ${BUILD_DIR} ${SRC_DIR}
    cmake --build ${BUILD_DIR} --config Release --target install -- -j${CPU_CORES}
else
    # Debug vs Release has no difference!!!
    BUILD_DIR=${BUILD_ROOT}/x64-Debug
    INSTALL_DIR=${INSTALL_ROOT}/x64-Debug
    cmake ${CMAKE_CONFIG} -D CMAKE_BUILD_TYPE=Debug -D CMAKE_INSTALL_PREFIX=${INSTALL_DIR} -B ${BUILD_DIR} ${SRC_DIR}
    cmake --build ${BUILD_DIR} --config Debug --target install -- -j${CPU_CORES}

    BUILD_DIR=${BUILD_ROOT}/x64-Release
    INSTALL_DIR=${INSTALL_ROOT}/x64-Release
    cmake ${CMAKE_CONFIG} -D CMAKE_BUILD_TYPE=Debug -D CMAKE_INSTALL_PREFIX=${INSTALL_DIR} -B ${BUILD_DIR} ${SRC_DIR}
    cmake --build ${BUILD_DIR} --config Release --target install -- -j${CPU_CORES}
fi
