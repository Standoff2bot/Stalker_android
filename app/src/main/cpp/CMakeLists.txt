cmake_minimum_required(VERSION 3.18)
project(xray-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point to the existing OpenXRay source
set(XRAY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../xray-16)

# Include existing OpenXRay CMake modules
list(APPEND CMAKE_MODULE_PATH "${XRAY_SOURCE_DIR}/cmake")

# Android-specific settings
set(ANDROID_PLATFORM android-24)
set(ANDROID_STL c++_shared)

# OpenGL ES 3.0 instead of desktop OpenGL
add_definitions(-DUSE_OGL -DANDROID)

# Find required libraries
find_library(log-lib log)
find_library(android-lib android)
find_library(EGL-lib EGL)
find_library(GLESv3-lib GLESv3)

# Include the main OpenXRay source tree
include_directories(
    ${XRAY_SOURCE_DIR}/src
    ${XRAY_SOURCE_DIR}/Externals
    ${XRAY_SOURCE_DIR}/Externals/SDL2/include
    ${XRAY_SOURCE_DIR}/Externals/OpenAL/include
    ${XRAY_SOURCE_DIR}/Externals/LuaJIT/src
)

# Collect OpenXRay source files
file(GLOB_RECURSE XRAY_ENGINE_SOURCES
    ${XRAY_SOURCE_DIR}/src/xrEngine/*.cpp
    ${XRAY_SOURCE_DIR}/src/xrCore/*.cpp
    ${XRAY_SOURCE_DIR}/src/xrCDB/*.cpp
    ${XRAY_SOURCE_DIR}/src/xrSound/*.cpp
    ${XRAY_SOURCE_DIR}/src/xrScriptEngine/*.cpp
    ${XRAY_SOURCE_DIR}/src/xrNetServer/*.cpp
    ${XRAY_SOURCE_DIR}/src/xrPhysics/*.cpp
)

file(GLOB_RECURSE XRAY_RENDER_SOURCES
    ${XRAY_SOURCE_DIR}/src/Layers/xrRender/*.cpp
    ${XRAY_SOURCE_DIR}/src/Layers/xrRenderPC_GL/*.cpp
)

file(GLOB_RECURSE XRAY_GAME_SOURCES
    ${XRAY_SOURCE_DIR}/src/xrGame/*.cpp
)

# JNI bridge
set(JNI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/jni_bridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/android_engine_wrapper.cpp
)

# Build the main engine library
add_library(xray-engine SHARED
    ${JNI_SOURCES}
    ${XRAY_ENGINE_SOURCES}
    ${XRAY_RENDER_SOURCES}
    ${XRAY_GAME_SOURCES}
)

# Link libraries
target_link_libraries(xray-engine
    ${log-lib}
    ${android-lib}
    ${EGL-lib}
    ${GLESv3-lib}
    # Pre-built OpenXRay dependencies from Termux build
    ${XRAY_SOURCE_DIR}/bin/aarch64/Release/libxrCore.so
    ${XRAY_SOURCE_DIR}/bin/aarch64/Release/libxrCDB.so
    ${XRAY_SOURCE_DIR}/bin/aarch64/Release/libxrSound.so
    ${XRAY_SOURCE_DIR}/bin/aarch64/Release/libxrAPI.so
    ${XRAY_SOURCE_DIR}/bin/aarch64/Release/libxrScriptEngine.so
    luajit-5.1
    SDL2
    openal
    ogg
    vorbis
    vorbisfile
    theora
    crypto
    lzo2
    jpeg
)

# Compiler flags for OpenXRay
target_compile_options(xray-engine PRIVATE
    -Wno-deprecated-declarations
    -Wno-unused-variable
    -Wno-unused-function
    -fno-strict-aliasing
    -DLINUX
    -D_MT
    -D_LINUX
)
