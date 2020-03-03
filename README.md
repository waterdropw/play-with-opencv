# Play with OpenCV

## Build & Install

### Build Status

| Linux                        |
|------------------------------|
| [![linux build status][1]][2]|

[1]: https://travis-ci.org/xbwee1024/play-with-halide.svg?branch=master
[2]: https://travis-ci.org/xbwee1024/play-with-halide

### Prerequisites
`Ninja`, `llvm` and `cmake` installed

For Android,
`Android NDK` installed. `Android SDK` is not required now (build Android examples is disabled)

### Clone the source code
```bash
git clone https://github.com/xbwee1024/play-with-opencv.git
cd play-with-opencv
git submodule update --init --recursive
# or clone with --recursive
git clone https://github.com/xbwee1024/play-with-opencv.git --recursive
```

### Build OpenCV
For Unix-like systems, 
```bash
./script/build-ocv.sh
# or add android to cross-compile Android
./script/build-ocv.sh android
```

For windows,
```bash
.\script\build-ocv.bat
# or add android to cross-compile Android
.\script\build-ocv.bat android
```

this will build the `OpenCV` library and install into `out/opencv/distrib/${TARGET_OS}`

Build Options:

```bash
CMAKE_CONFIG="-G "Ninja" \\
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
```
- `BUILD_SHARED_LIBS`: Set `On` for shared library, `OFF` for static library
- `OPENCV_EXTRA_MODULES_PATH`: Build with contrib libraries, if no need, remove it.
- `BUILD_opencv_world`: Set `ON` will output only one `libopencv_world.so`
- `WITH_OPENCL`: Enable/disable OpenCL support
- `WITH_OPENMP`: Enable/Disable OpenMP support
- `WITH_TBB`: Enable/Disable TBB support
- `WITH_HALIDE`: Enable/Disable Halide support

### Build Project

```bash
./script/build.sh
# or add android to cross-compile Android
./script/build.sh android
```

When building finished, binaries will be installed at `out/install/${TARGET_OS}/bin`


## Use OpenCV

1. set/export `OpenCV_DIR` to the right path of OpenCV, for Android:

```bash
export OpenCV_DIR=out/install/Android/native/jni
```

2. Call find_package in the user target's CMakeLists.txt

```cmake
find_package(OpenCV 4 REQUIRED)

target_link_libraries(your_target
    PRIVATE
        ${OpenCV_LIBS}
)
```
