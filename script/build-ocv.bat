@echo OFF

set TARGET_OS=%1

set HOST_OS=Windows
set CPU_CORES=4
if "%TARGET_OS%" == "" (
    set TARGET_OS=%HOST_OS%
)
if %TARGET_OS% == android (
    set TARGET_OS=Android
)

set BUILD_ROOT=.\out\opencv\build\%TARGET_OS%
set INSTALL_DIR=.\out\opencv\distrib\%TARGET_OS%

set SRC_DIR=.\external\opencv
set EXTRA_MODULE_PATH=.\external\opencv_contrib\modules
set BUILD_DIR_DEBUG=%BUILD_ROOT%\x64-Debug
set BUILD_DIR_RELEASE=%BUILD_ROOT%\x64-Release
set ANDROID_BUILD_32=%BUILD_ROOT%\arm
set ANDROID_BUILD_64=%BUILD_ROOT%\aach64


echo *** *** *** *** *** *** *** *** *** *** *** ***
echo HOST_OS: %HOST%
echo TARGET_OS: %TARGET_OS%
echo CPU_CORES: %CPU_CORES%
echo SRC Dir: %SRC_DIR%
if %TARGET_OS% == %HOST_OS% (
    echo Build Dir [Debug]: %BUILD_DIR_DEBUG%
    echo Build Dir [Release]: %BUILD_DIR_RELEASE%
) else (
    echo Build Dir [Android32]: %ANDROID_BUILD_32%
    echo Build Dir [Android64]: %ANDROID_BUILD_64%
)
echo Install Dir: %INSTALL_DIR%
echo *** *** *** *** *** *** *** *** *** *** *** ***


if %TARGET_OS% == Android (
    :: armeabi-v7a
    cmake  -G "Ninja" ^
        -D CMAKE_BUILD_TYPE=Release ^
        -D CMAKE_INSTALL_PREFIX=%INSTALL_DIR%\arm ^
        -D CMAKE_POSITION_INDEPENDENT_CODE=ON ^
        -D OPENCV_EXTRA_MODULES_PATH=%EXTRA_MODULE_PATH% ^
        -D ENABLE_CXX11=ON ^
        -D BUILD_SHARED_LIBS=ON ^
        -D BUILD_opencv_world=OFF ^
        -D BUILD_TESTS=OFF ^
        -D BUILD_EXAMPLES=ON ^
        -D BUILD_JAVA=ON ^
        -D BUILD_opencv_python2=OFF ^
        -D BUILD_opencv_python_bindings_generator=OFF ^
        -D BUILD_opencv_python_tests=OFF ^
        -D BUILD_opencv_java=ON ^
        -D BUILD_opencv_java_bindings_generator=ON ^
        -D WITH_OPENCL=ON ^
        -D WITH_OPENMP=ON ^
        -D WITH_HALIDE=ON ^
        -D WITH_TBB=ON ^
        -D BUILD_ANDROID_PROJECTS=OFF ^
        -D BUILD_opencv_freetype=OFF ^
        -D CMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\build\cmake\android.toolchain.cmake ^
        -D ANDROID_SDK=%ANDROID_SDK% ^
        -D ANDROID_NDK=%ANDROID_NDK% ^
        -D ANDROID_PLATFORM=android-28 ^
        -D ANDROID_STL=c++_static ^
        -D ANDROID_PIE=ON ^
        -D ANDROID_ABI="armeabi-v7a with NEON" ^
        -B %ANDROID_BUILD_32% %SRC_DIR%

    cmake --build %ANDROID_BUILD_32% --config Release --target install -- -j%CPU_CORES%
    
    :: arm64-v8a
    cmake  -G "Ninja" ^
        -D CMAKE_BUILD_TYPE=Release ^
        -D CMAKE_INSTALL_PREFIX=%INSTALL_DIR%\aarch64 ^
        -D CMAKE_POSITION_INDEPENDENT_CODE=ON ^
        -D OPENCV_EXTRA_MODULES_PATH=%EXTRA_MODULE_PATH% ^
        -D ENABLE_CXX11=ON ^
        -D BUILD_SHARED_LIBS=ON ^
        -D BUILD_opencv_world=OFF ^
        -D BUILD_TESTS=OFF ^
        -D BUILD_EXAMPLES=ON ^
        -D BUILD_JAVA=ON ^
        -D BUILD_opencv_python2=OFF ^
        -D BUILD_opencv_python_bindings_generator=OFF ^
        -D BUILD_opencv_python_tests=OFF ^
        -D BUILD_opencv_java=ON ^
        -D BUILD_opencv_java_bindings_generator=ON ^
        -D WITH_OPENCL=ON ^
        -D WITH_OPENMP=ON ^
        -D WITH_HALIDE=ON ^
        -D WITH_TBB=ON ^
        -D BUILD_ANDROID_PROJECTS=OFF ^
        -D BUILD_opencv_freetype=OFF ^
        -D CMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\build\cmake\android.toolchain.cmake ^
        -D ANDROID_SDK=%ANDROID_SDK% ^
        -D ANDROID_NDK=%ANDROID_NDK% ^
        -D ANDROID_PLATFORM=android-28 ^
        -D ANDROID_STL=c++_static ^
        -D ANDROID_PIE=ON ^
        -D ANDROID_ABI="arm64-v8a" ^
        -B %ANDROID_BUILD_64% %SRC_DIR%

    cmake --build %ANDROID_BUILD_64% --config Release --target install -- -j%CPU_CORES%

) else (
    :: Debug
    REM cmake -G "Ninja" ^
    cmake -G "Visual Studio 16 2019" -A x64 ^
        -D CMAKE_BUILD_TYPE=Debug ^
        -D CMAKE_INSTALL_PREFIX=%INSTALL_DIR%\x64-Debug ^
        -D CMAKE_POSITION_INDEPENDENT_CODE=ON ^
        -D OPENCV_EXTRA_MODULES_PATH=%EXTRA_MODULE_PATH% ^
        -D ENABLE_CXX11=ON ^
        -D BUILD_SHARED_LIBS=ON ^
        -D BUILD_opencv_world=OFF ^
        -D BUILD_TESTS=OFF ^
        -D BUILD_EXAMPLES=ON ^
        -D BUILD_JAVA=ON ^
        -D BUILD_opencv_python2=OFF ^
        -D BUILD_opencv_python_bindings_generator=OFF ^
        -D BUILD_opencv_python_tests=OFF ^
        -D BUILD_opencv_java=ON ^
        -D BUILD_opencv_java_bindings_generator=ON ^
        -D WITH_OPENCL=ON ^
        -D WITH_OPENMP=ON ^
        -D WITH_HALIDE=ON ^
        -D WITH_TBB=ON ^
        -D WITH_IPP=ON ^
        -B %BUILD_DIR_DEBUG%  %SRC_DIR%

    cmake --build %BUILD_DIR_DEBUG% --config Debug --target install

    :: Release
    REM cmake -G "Ninja" ^
    REM cmake -G "Visual Studio 16 2019" -A x64 ^
    REM     -D CMAKE_BUILD_TYPE=Release ^
    REM     -D CMAKE_INSTALL_PREFIX=%INSTALL_DIR%\x64-Release ^
    REM     -D CMAKE_POSITION_INDEPENDENT_CODE=ON ^
    REM     -D OPENCV_EXTRA_MODULES_PATH=%EXTRA_MODULE_PATH% ^
    REM     -D ENABLE_CXX11=ON ^
    REM     -D BUILD_SHARED_LIBS=ON ^
    REM     -D BUILD_opencv_world=OFF ^
    REM     -D BUILD_TESTS=OFF ^
    REM     -D BUILD_EXAMPLES=ON ^
    REM     -D BUILD_JAVA=ON ^
    REM     -D BUILD_opencv_python2=OFF ^
    REM     -D BUILD_opencv_python_bindings_generator=OFF ^
    REM     -D BUILD_opencv_python_tests=OFF ^
    REM     -D BUILD_opencv_java=ON ^
    REM     -D BUILD_opencv_java_bindings_generator=ON ^
    REM     -D WITH_OPENCL=ON ^
    REM     -D WITH_OPENMP=ON ^
    REM     -D WITH_HALIDE=ON ^
    REM     -D WITH_TBB=ON ^
    REM     -D WITH_IPP=ON ^
    REM     -B %BUILD_DIR_RELEASE% %SRC_DIR%
    
    REM cmake --build %BUILD_DIR_RELEASE% --config Release --target install
)
