@echo OFF

set TARGET_OS=%1
if "%TARGET_OS%" == "" (
    set TARGET_OS=%HOST_OS%
)
if %TARGET_OS% == android (
    set TARGET_OS=Android
)
set HOST_OS=Windows
set CPU_CORES=4

set BUILD_DIR_DEBUG=.\out\build\x64-Debug
set BUILD_DIR_RELEASE=.\out\build\x64-Release
set ANDROID_BUILD_32=.\out\build\arm
set ANDROID_BUILD_64=.\out\build\aach64
set INSTALL_DIR=.\out\install\%TARGET_OS%


echo *** *** *** *** *** *** *** *** *** *** *** ***
echo HOST_OS: %HOST%
echo TARGET_OS: %TARGET_OS%
echo CPU_CORES: %CPU_CORES%

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
    :: 32bit
    cmake  -G "Ninja" ^
        -D CMAKE_INSTALL_PREFIX=%INSTALL_DIR% ^
        -D CMAKE_POSITION_INDEPENDENT_CODE=ON ^
        -D CMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\build\cmake\android.toolchain.cmake ^
        -D ANDROID_SDK=%ANDROID_SDK% ^
        -D ANDROID_NDK=%ANDROID_NDK% ^
        -D ANDROID_PLATFORM=android-28 ^
        -D ANDROID_STL=c++_static ^
        -D ANDROID_PIE=ON ^
        -D ANDROID_ABI="armeabi-v7a with NEON" ^
        -B %ANDROID_BUILD_32% .

    cmake --build %ANDROID_BUILD_32% --config Release --target install -- -j%CPU_CORES%
    :: 64bit
    cmake  -G "Ninja" ^
        -D CMAKE_INSTALL_PREFIX=%INSTALL_DIR% ^
        -D CMAKE_POSITION_INDEPENDENT_CODE=ON ^
        -D CMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\build\cmake\android.toolchain.cmake ^
        -D ANDROID_SDK=%ANDROID_SDK% ^
        -D ANDROID_NDK=%ANDROID_NDK% ^
        -D ANDROID_PLATFORM=android-28 ^
        -D ANDROID_STL=c++_static ^
        -D ANDROID_PIE=ON ^
        -D ANDROID_ABI="arm64-v8a" ^
        -B %ANDROID_BUILD_64% .

    cmake --build %ANDROID_BUILD_64% --config Release --target install -- -j%CPU_CORES%

) else (

    cmake -S %SRC_DIR% -B %BUILD_DIR_DEBUG% ^
        -G "Visual Studio 16 2019" ^
        -D CMAKE_POSITION_INDEPENDENT_CODE=ON ^
        -D CMAKE_INSTALL_PREFIX=%INSTALL_DIR% ^
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
        -D WITH_IPP=ON
    
    cmake --build %BUILD_DIR_DEBUG% --config Debug --target install


    cmake -S %SRC_DIR% -B %BUILD_DIR_RELEASE% ^
        -G "Visual Studio 16 2019" ^
        -D CMAKE_POSITION_INDEPENDENT_CODE=ON ^
        -D CMAKE_INSTALL_PREFIX=%INSTALL_DIR% ^
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
        -D WITH_IPP=ON
    
    cmake --build %BUILD_DIR_RELEASE% --config Release --target install
)