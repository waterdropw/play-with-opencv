# Play with OpenCV

## User Guide

GCC/Clang, CMake, Android-sdk, Android-ndk installed  
for Linux, Mac, Windows

```bash
./build.sh
```

for Android

```bash
./build.sh android
```

After build complete, the binary is located at 
```bash
out/install/bin
```

## Build OpenCV

### Features

1. Convienient build script for multiple platform, especially for Android

2. Remove the dependency of libgnustl_shared.so, instead with c++_static

3. Enable some 3rdparty library for performance,  TBB/IPP, OpenCL, IPP, NEON(Android)

4. Build with contrib libraries  
opencv_contrib are some extra usefull libraries which does not build within official release SDK  

### Prerequisites
**Ninja** installed

For Unix-like systems,
**gcc** or **clang** installed

For windows,
**Visual Studio** installed

For Android,
**Android NDK** installed. **Android SDK** is not required now (build Android examples is disabled)

### Building
For Unix-like systems, 
```bash
./script/build-ocv.sh
# or add android to cross-compile Android
./script/build-ocv.sh android
```
this will build the **OpenCV** library and install into /usr/local/

For windows,
```bash
.\script\build-ocv.bat
# or add android to cross-compile Android
.\script\build-ocv.bat android
```

### Use OpenCV

1. set/export **OpenCV_DIR** to the right path of OpenCV, for Android:

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
