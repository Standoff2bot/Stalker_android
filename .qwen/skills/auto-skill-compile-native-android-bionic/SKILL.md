---
name: compile-native-android-bionic
description: Compile C++ projects natively on Android Bionic (Termux) without NDK cross-compilation, fixing common issues with headers, toolchain flags, and library linking
source: auto-skill
extracted_at: '2026-08-14T17:00:46.022Z'
---

# Native Android Bionic Compilation in Termux

This skill captures the approach for compiling large C++ projects (like OpenXRay game engine) natively on Android Bionic using Termux, without relying on Android NDK cross-compilation or PRoot/glibc.

## When to Use

- Compiling C++ projects directly on Android devices in Termux
- Projects originally designed for Linux/glibc that need Bionic adaptation
- When encountering "cannot locate symbol" or missing header errors during native Android compilation
- When Android NDK-style cross-compilation fails or is unavailable

## Core Principle

**Use native Termux compilation instead of Android NDK-style cross-compilation.** Remove `--target` and `--sysroot` compiler flags that force Clang to search for NDK-style headers. Let the compiler use Termux's native environment.

## Common Issues and Solutions

### 1. Missing Kernel Headers (asm/types.h)

**Symptom:** `fatal error: 'asm/types.h' file not found` from `linux/types.h`

**Root cause:** Termux kernel headers are in `/usr/include/aarch64-linux-android/asm/` but code looks for `/usr/include/asm/`

**Solution:**
```bash
ln -sf aarch64-linux-android/asm /data/data/com.termux/files/usr/include/asm
```

### 2. Missing C++ Standard Library Headers

**Symptom:** `fatal error: 'cstring' file not found`, `'algorithm' file not found`, etc.

**Root cause:** Using `CMAKE_C_COMPILER_TARGET` and `CMAKE_SYSROOT` makes Clang search for Android NDK-style headers instead of native Termux libc++ headers

**Solution in CMake toolchain:**
```cmake
# Comment out NDK-style flags
# set(CMAKE_C_COMPILER_TARGET aarch64-unknown-linux-android24)
# set(CMAKE_CXX_COMPILER_TARGET aarch64-unknown-linux-android24)
# set(CMAKE_SYSROOT ${TERMUX_PREFIX})

# Keep these for native compilation
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
```

### 3. Missing OpenGL Symbols at Runtime

**Symptom:** `CANNOT LINK EXECUTABLE: cannot locate symbol "glGetString"` or similar OpenGL functions

**Root cause:** Renderer library not linked with OpenGL ES libraries

**Solution in CMakeLists.txt:**
```cmake
target_link_libraries(YourRenderer
    PRIVATE
    # ... other libs ...
    GLESv2    # OpenGL ES 2/3
    EGL       # EGL for context creation
)
```

**Verify with:**
```bash
readelf -d libYourRenderer.so | grep NEEDED
# Should show: libGLESv2.so.2 and libEGL.so.1
```

### 4. LuaJIT vs Lua 5.4 API Incompatibility

**Symptom:** `error: use of undeclared identifier 'LUA_GCTIMEOUT'`

**Root cause:** LuaJIT is based on Lua 5.1 API. `LUA_GCTIMEOUT` is a Lua 5.4+ garbage collection mode.

**Solution:**
```cpp
#ifdef LUA_GCTIMEOUT
    if (lua_gc(L, LUA_GCTIMEOUT, timeout) >= 0)
        break;
    // LUA_GCTIMEOUT unsupported, fallback
    [[fallthrough]];
#else
    // LuaJIT doesn't support LUA_GCTIMEOUT, use LUA_GCSTEP
    [[fallthrough]];
#endif
case LUA_GCSTEP:
    lua_gc(L, LUA_GCSTEP, stepsize);
```

## CMake Toolchain Template

```cmake
# Android Bionic Native Toolchain for Termux
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Compiler (no --target flags for native compilation)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Termux paths
set(TERMUX_PREFIX "/data/data/com.termux/files/usr")
set(CMAKE_FIND_ROOT_PATH ${TERMUX_PREFIX})

# Search paths
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Bionic-specific flags
set(CMAKE_C_FLAGS_INIT "-fPIC -fstack-protector-strong")
set(CMAKE_CXX_FLAGS_INIT "-fPIC -fstack-protector-strong")

# Use libc++ (Android's C++ stdlib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")

# ARM64 optimizations
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a")

# OpenGL ES paths (use Mesa on Termux)
set(GLES2_INCLUDE_DIR "${TERMUX_PREFIX}/include")
set(GLES2_gl_LIBRARY "${TERMUX_PREFIX}/lib/libGLESv2.so")
set(EGL_LIBRARY "${TERMUX_PREFIX}/lib/libEGL.so")
```

## Build Configuration Tips

1. **Use Ninja generator** for faster parallel builds:
   ```bash
   cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ...
   ```

2. **Enable LTO cautiously** - Link Time Optimization is very slow on ARM devices:
   ```cmake
   # LTO will make linking take 20-30+ minutes for large libraries
   set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
   ```

3. **Monitor memory during linking** - Large C++ projects can use 1+ GB during LTO linking:
   ```bash
   ps aux | grep ld.lld  # Check linker memory usage
   ```

## Verification Steps

After successful build:

1. **Check binary dependencies:**
   ```bash
   readelf -d ./your_binary | grep NEEDED
   # Should show Bionic libs: libc.so, libm.so, libc++_shared.so
   # NOT glibc: libstdc++.so.6, libpthread.so.0
   ```

2. **Test basic startup:**
   ```bash
   ./your_binary --version
   # Should print version without "cannot locate symbol" errors
   ```

3. **Verify OpenGL ES linking (for graphics apps):**
   ```bash
   readelf -d ./libYourRenderer.so | grep -E "GLESv2|EGL"
   # Must show libGLESv2.so.2 and libEGL.so.1
   ```

## Why Native vs NDK Cross-Compilation?

**Native Termux compilation:**
- ✅ Uses system libc++ headers directly
- ✅ No sysroot confusion
- ✅ Simpler toolchain setup
- ✅ All Termux packages work out of box
- ❌ Slower on mobile hardware

**NDK cross-compilation:**
- ✅ Faster on desktop
- ✅ Consistent build environment
- ❌ Requires Android NDK setup
- ❌ Header path mismatches with Termux packages
- ❌ More complex toolchain configuration

For projects that need to run on Termux with Termux dependencies (SDL2, OpenAL, Mesa, etc.), native compilation is more reliable.

## Real-World Example: OpenXRay Engine

Successfully compiled OpenXRay (S.T.A.L.K.E.R. game engine) on Android Bionic:
- 19 shared libraries (98 MB total)
- xrGame.so (53 MB, 1900+ object files)
- OpenGL ES 3.2 renderer with Mesa Panfrost
- LuaJIT scripting engine
- Build time: ~2 hours on ARM64 (8-core)
- Link time for xrGame.so: 30+ minutes with LTO

All fixes applied: kernel headers symlink, removed NDK flags, added OpenGL ES linking, LuaJIT API compatibility patches.
