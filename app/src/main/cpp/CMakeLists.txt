cmake_minimum_required(VERSION 3.22)
project(xray-android)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# OpenXRay source root
set(XRAY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../xray-16/src)
set(XRAY_EXTERNALS ${CMAKE_CURRENT_SOURCE_DIR}/../../../../xray-16/Externals)
set(XRAY_SDK ${CMAKE_CURRENT_SOURCE_DIR}/../../../../xray-16/sdk)

# Platform defines
add_compile_definitions(
    ANDROID
    LINUX
    _LINUX
    XR_PLATFORM_LINUX
    XRAY_STATIC_BUILD
)

# Build configuration (needed for Platform.hpp line 108 check)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
else()
    add_compile_definitions(NDEBUG)
endif()

# Android compatibility layer (replaces SDL2 dependencies)
add_compile_definitions(
    SDL_stdinc_h_=1
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${XRAY_SRC}
    ${XRAY_SRC}/xrCore
    ${XRAY_SRC}/xrEngine
    ${XRAY_SRC}/xrCDB
    ${XRAY_SRC}/xrSound
    ${XRAY_SRC}/xrGame
    ${XRAY_SRC}/xrServerEntities
    ${XRAY_SRC}/xrNetServer
    ${XRAY_SRC}/xrScriptEngine
    ${XRAY_SRC}/xrPhysics
    ${XRAY_SRC}/xrParticles
    ${XRAY_SRC}/xrUICore
    ${XRAY_SRC}/Layers
    ${XRAY_SRC}/Common
    ${XRAY_SRC}/Include
    ${XRAY_EXTERNALS}/tracy/public
    ${XRAY_EXTERNALS}/LuaJIT/src
    ${XRAY_EXTERNALS}/luabind
    ${XRAY_SDK}/include
)

# Find Android libraries
find_library(log-lib log)
find_library(android-lib android)
find_library(EGL-lib EGL)
find_library(GLESv3-lib GLESv3)

# xrCore - critical base library
file(GLOB_RECURSE XRCORE_SOURCES
    ${XRAY_SRC}/xrCore/*.cpp
    ${XRAY_SRC}/xrCommon/*.cpp
)

# Exclude Windows-specific and editor-only files
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*_win32.*")
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*Windows.*")
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*dx.*")
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*Editor.*")
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*BoneEditor.*")

add_library(xrCore STATIC ${XRCORE_SOURCES})
target_compile_options(xrCore PRIVATE
    -DXRCORE_EXPORTS
    -Wno-deprecated-declarations
    -fno-strict-aliasing
    -fexceptions
)

# xrCDB - collision detection
file(GLOB_RECURSE XRCDB_SOURCES ${XRAY_SRC}/xrCDB/*.cpp)
add_library(xrCDB STATIC ${XRCDB_SOURCES})
target_link_libraries(xrCDB PRIVATE xrCore)

# xrScriptEngine - Lua integration
file(GLOB_RECURSE XRSCRIPT_SOURCES ${XRAY_SRC}/xrScriptEngine/*.cpp)
add_library(xrScriptEngine STATIC ${XRSCRIPT_SOURCES})
target_link_libraries(xrScriptEngine PRIVATE xrCore)

# xrSound stub (OpenAL integration needed)
file(GLOB_RECURSE XRSOUND_SOURCES ${XRAY_SRC}/xrSound/*.cpp)
list(FILTER XRSOUND_SOURCES EXCLUDE REGEX ".*OpenALDeviceList.*")
add_library(xrSound STATIC ${XRSOUND_SOURCES})
target_link_libraries(xrSound PRIVATE xrCore)

# xrEngine - main engine
file(GLOB_RECURSE XRENGINE_SOURCES ${XRAY_SRC}/xrEngine/*.cpp)
list(FILTER XRENGINE_SOURCES EXCLUDE REGEX ".*xrRender.*")
list(FILTER XRENGINE_SOURCES EXCLUDE REGEX ".*Device_create.*")
add_library(xrEngine STATIC ${XRENGINE_SOURCES})
target_link_libraries(xrEngine PRIVATE 
    xrCore
    xrCDB
    xrSound
)

# xrServerEntities
file(GLOB_RECURSE XRSERVER_SOURCES ${XRAY_SRC}/xrServerEntities/*.cpp)
add_library(xrServerEntities STATIC ${XRSERVER_SOURCES})
target_link_libraries(xrServerEntities PRIVATE xrCore)

# xrGame - game logic
file(GLOB_RECURSE XRGAME_SOURCES ${XRAY_SRC}/xrGame/*.cpp)
# Exclude UI for now (complex dependency)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*ui/.*")
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*UI.*")
add_library(xrGame STATIC ${XRGAME_SOURCES})
target_link_libraries(xrGame PRIVATE
    xrCore
    xrEngine
    xrCDB
    xrServerEntities
    xrScriptEngine
)

# Main JNI wrapper library
add_library(xray-engine SHARED
    jni_bridge.cpp
    android_main.cpp
)

target_link_libraries(xray-engine
    PRIVATE
    xrCore
    xrCDB
    xrEngine
    xrSound
    xrScriptEngine
    xrServerEntities
    xrGame
    ${log-lib}
    ${android-lib}
    ${EGL-lib}
    ${GLESv3-lib}
)

target_compile_options(xray-engine PRIVATE
    -Wno-deprecated-declarations
    -fno-strict-aliasing
    -fexceptions
)
