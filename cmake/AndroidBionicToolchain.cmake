# Android Bionic Toolchain for Termux
# Target: aarch64-unknown-linux-android24

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_SYSTEM_VERSION 1)

# Android API level
set(ANDROID_PLATFORM android-24)
set(ANDROID_ABI arm64-v8a)

# Compiler settings
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
# Remove --target flag to use native Termux compilation instead of NDK-style
# set(CMAKE_C_COMPILER_TARGET aarch64-unknown-linux-android24)
# set(CMAKE_CXX_COMPILER_TARGET aarch64-unknown-linux-android24)

# Termux prefix
set(TERMUX_PREFIX "/data/data/com.termux/files/usr")
set(CMAKE_FIND_ROOT_PATH ${TERMUX_PREFIX})
# Remove sysroot to use native Termux compilation
# set(CMAKE_SYSROOT ${TERMUX_PREFIX})

# Search paths
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Bionic specific flags
set(CMAKE_C_FLAGS_INIT "-fPIC -fstack-protector-strong")
set(CMAKE_CXX_FLAGS_INIT "-fPIC -fstack-protector-strong")

# Link flags for Bionic
set(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,--enable-new-dtags -Wl,--gc-sections -Wl,-rpath=${TERMUX_PREFIX}/lib")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-Wl,--enable-new-dtags -Wl,--gc-sections")

# Use libc++ instead of libstdc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")

# ARM64 specific optimizations
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a")

# Use OpenGL ES instead of desktop GL
set(OPENGL_INCLUDE_DIR "${TERMUX_PREFIX}/include")
set(OPENGL_gl_LIBRARY "${TERMUX_PREFIX}/lib/libGL.so")
set(EGL_INCLUDE_DIR "${TERMUX_PREFIX}/include")
set(EGL_LIBRARY "${TERMUX_PREFIX}/lib/libEGL.so")
set(GLES2_INCLUDE_DIR "${TERMUX_PREFIX}/include")
set(GLES2_gl_LIBRARY "${TERMUX_PREFIX}/lib/libGLESv2.so")
set(GLES3_INCLUDE_DIR "${TERMUX_PREFIX}/include")
set(GLES3_gl_LIBRARY "${TERMUX_PREFIX}/lib/libGLESv2.so")

# pkg-config
set(PKG_CONFIG_EXECUTABLE "${TERMUX_PREFIX}/bin/pkg-config")
set(ENV{PKG_CONFIG_PATH} "${TERMUX_PREFIX}/lib/pkgconfig:${TERMUX_PREFIX}/share/pkgconfig")

# Install prefix
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${TERMUX_PREFIX}" CACHE PATH "Install prefix" FORCE)
endif()
