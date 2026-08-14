#pragma once

// Minimal SDL stub for Android - provides only the functions used by LocatorAPI.cpp
// Real SDL2 not needed for basic file system operations

#include <cstdlib>
#include <cstring>
#include <cstdio>

// SDL string functions - map to standard C
#define SDL_strlen strlen
#define SDL_strlcpy(dst, src, size) strncpy(dst, src, size)
#define SDL_free free
#define SDL_malloc malloc

// SDL_GetPrefPath - returns Android-specific user data directory
// For Android, we use /sdcard/Android/data/com.openxray.stalker/files/
inline char* SDL_GetPrefPath(const char* org, const char* app) {
    // Simplified: return hardcoded Android path
    // Real implementation would get this from Java via JNI
    const char* path = "/sdcard/Android/data/com.openxray.stalker/files/";
    char* result = (char*)malloc(strlen(path) + 1);
    if (result) {
        strcpy(result, path);
    }
    return result;
}
