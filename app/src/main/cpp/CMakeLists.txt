cmake_minimum_required(VERSION 3.22)
project(xray-android-jni)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# OpenXRay source directory
set(XRAY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../xray-16)

# Include OpenXRay's CMake modules
list(APPEND CMAKE_MODULE_PATH "${XRAY_ROOT}/cmake")

# Android and OpenGL ES definitions
add_definitions(
    -DANDROID
    -DLINUX
    -D_LINUX
    -D_MT
    -DUSE_OGL
)

# Configure OpenXRay build options
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(XRAY_USE_LUAJIT ON CACHE BOOL "" FORCE)
set(XRAY_USE_ASAN OFF CACHE BOOL "" FORCE)
set(XRAY_ENABLE_TRACY OFF CACHE BOOL "" FORCE)

# Build OpenXRay first
add_subdirectory(${XRAY_ROOT} ${CMAKE_BINARY_DIR}/xray-build EXCLUDE_FROM_ALL)

# Find Android libraries
find_library(log-lib log)
find_library(android-lib android)
find_library(EGL-lib EGL)
find_library(GLESv3-lib GLESv3)

# JNI wrapper source
add_library(xray-engine SHARED
    jni_bridge.cpp
    android_main.cpp
)

# Link with OpenXRay and Android libraries
target_link_libraries(xray-engine
    PRIVATE
    xrEngine
    xrGame
    ${log-lib}
    ${android-lib}
    ${EGL-lib}
    ${GLESv3-lib}
)

# Include directories
target_include_directories(xray-engine PRIVATE
    ${XRAY_ROOT}/src
    ${XRAY_ROOT}/Externals
)

target_compile_options(xray-engine PRIVATE
    -Wno-deprecated-declarations
    -fno-strict-aliasing
)
