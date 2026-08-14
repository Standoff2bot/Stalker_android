cmake_minimum_required(VERSION 3.22)
project(xray-android-wrapper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find Android libraries
find_library(log-lib log)
find_library(android-lib android)
find_library(EGL-lib EGL)
find_library(GLESv3-lib GLESv3)

# For now, create a minimal working JNI library
# We'll integrate OpenXRay components step by step

add_library(xray-engine SHARED
    jni_bridge.cpp
    android_main_simple.cpp
)

target_link_libraries(xray-engine
    PRIVATE
    ${log-lib}
    ${android-lib}
    ${EGL-lib}
    ${GLESv3-lib}
)

target_compile_options(xray-engine PRIVATE
    -DANDROID
    -DLINUX
    -D_LINUX
    -Wno-deprecated-declarations
    -fno-strict-aliasing
)
