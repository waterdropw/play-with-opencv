#!/usr/bin/env bash

CURRENT_DIR=`pwd`
HOST=`uname -sm`
TARGET_OS=$1
TARGET_ARCH=$2
HOST_SYSTEM=`uname`
CPU_CORES=4
if [[ -z ${TARGET_OS} ]]; then
    TARGET_OS=${HOST_SYSTEM}
fi
if [[ -z ${TARGET_ARCH} ]]; then
    TARGET_ARCH=arm64
fi

if [[ ${TARGET_OS} == "Android" || ${TARGET_OS} == "android" ]]; then
    TARGET_OS="Android"
    if [[ ${TARGET_ARCH} == "arm" ]]; then
        TARGET_ARCH_ABI=armeabi-v7a
    elif [[ ${TARGET_ARCH} == "arm64" ]]; then
        TARGET_ARCH_ABI=arm64-v8a
    else
        echo "Unsupported Arch: ${TARGET_ARCH}"
    fi
fi

if [[ ${HOST_SYSTEM} == "Linux" ]]; then
    CPU_CORES=`cat /proc/cpuinfo |grep "processor"|wc -l`
elif [[ ${HOST_SYSTEM} == "Darwin" ]]; then
    CPU_CORES=`sysctl -n hw.physicalcpu`
fi



INSTALL_ROOT=out/install/${TARGET_OS}
BUILD_ROOT=out/build/${TARGET_OS}
if [[ ! -e "${OpenCV_DIR}" ]]; then
    if [[ ${TARGET_OS} == "Android" ]]; then
        export OpenCV_DIR=${CURRENT_DIR}/out/opencv/distrib/${TARGET_OS}/${TARGET_ARCH_ABI}/sdk/native/jni
    else
        export OpenCV_DIR=${CURRENT_DIR}/out/opencv/distrib/${TARGET_OS}/x64-Release/lib/cmake/opencv4
    fi
fi

echo "*** *** *** *** *** *** *** *** *** *** *** ***"
echo "HOST_OS: ${HOST}"
echo "TARGET_OS: ${TARGET_OS}"
echo "TARGET_ARCH: ${TARGET_ARCH}"
echo "TARGET_ARCH_ABI: ${TARGET_ARCH_ABI}"
echo "CPU_CORES: ${CPU_CORES}"
# echo "SRC Dir: ${SRC_DIR}"
echo "Build Dir: ${BUILD_ROOT}"
echo "Install Dir: ${INSTALL_ROOT}"
echo "OpenCV_DIR: ${OpenCV_DIR}"
echo "*** *** *** *** *** *** *** *** *** *** *** ***"


if [[ ${TARGET_OS} == "Android" ]]; then
    if [[ -d "$ANDROID_NDK" ]]; then
        echo "ANDROID_NDK=$ANDROID_NDK"
    elif [[ -d "$ANDROID_NDK_ROOT" ]]; then
        ANDROID_NDK=$ANDROID_NDK_ROOT
    elif [[ -d "$ANDROID_HOME" ]]; then
        ANDROID_NDK=$ANDROID_HOME/ndk-bundle
    fi
    
    if [[ -d "$ANDROID_SDK" ]]; then
        echo "ANDROID_SDK=$ANDROID_SDK"
    elif [[ -d "$ANDROID_HOME" ]]; then
        ANDROID_SDK=$ANDROID_HOME
    fi

    if [[ ! -e "${ANDROID_NDK}" ]]; then
        echo "Error: ANDROID_NDK DOES NOT EXISTS"
        exit
    fi
    if [[ ! -e "${ANDROID_SDK}" ]]; then
        echo "Error: ANDROID_SDK DOES NOT EXISTS"
        exit
    fi
fi

CMAKE_CONFIG="-G "Ninja" \\
    -D CMAKE_INSTALL_PREFIX=${INSTALL_DIR} \\
    -D CMAKE_POSITION_INDEPENDENT_CODE=ON \\
    -D CMAKE_PREFIX_PATH=${OpenCV_DIR} \\
    "

if [[ ${TARGET_OS} == "Android" ]]; then
    CMAKE_CONFIG="${CMAKE_CONFIG} \\
    -D CMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \\
    -D ANDROID_SDK=${ANDROID_SDK} \\
    -D ANDROID_NDK=${ANDROID_NDK} \\
    -D ANDROID_PLATFORM=android-28 \\
    -D ANDROID_STL=c++_static \\
    -D ANDROID_PIE=ON "
fi

if [[ ${TARGET_OS} == "Android" ]]; then

    BUILD_DIR=${BUILD_ROOT}/arm64-v8a
    INSTALL_DIR=${INSTALL_ROOT}/arm64-v8a
    cmake ${CMAKE_CONFIG} -D ANDROID_ABI="arm64-v8a" -D CMAKE_INSTALL_PREFIX=${INSTALL_DIR} -B ${BUILD_DIR} .
    cmake --build ${BUILD_DIR} --config Release --target install -- -j${CPU_CORES}

    # BUILD_DIR=${BUILD_ROOT}/armeabi-v7a
    # INSTALL_DIR=${INSTALL_ROOT}/armeabi-v7a
    # cmake ${CMAKE_CONFIG} -D ANDROID_ABI="armeabi-v7a with NEON" -D CMAKE_INSTALL_PREFIX=${INSTALL_DIR} -B ${BUILD_DIR} .
    # cmake --build ${BUILD_DIR} --config Release --target install -- -j${CPU_CORES}

else

    BUILD_DIR=${BUILD_ROOT}/x64-Release
    INSTALL_DIR=${INSTALL_ROOT}/x64-Release
    cmake ${CMAKE_CONFIG} -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=${INSTALL_DIR} -B ${BUILD_DIR} .
    cmake --build ${BUILD_DIR} --config Release --target install -- -j${CPU_CORES}

    # Debug vs Release has no difference!!!
    # BUILD_DIR=${BUILD_ROOT}/x64-Debug
    # INSTALL_DIR=${INSTALL_ROOT}/x64-Debug
    # cmake ${CMAKE_CONFIG} -D CMAKE_BUILD_TYPE=Debug -D CMAKE_INSTALL_PREFIX=${INSTALL_DIR} -B ${BUILD_DIR} .
    # cmake --build ${BUILD_DIR} --config Debug --target install -- -j${CPU_CORES}

fi
