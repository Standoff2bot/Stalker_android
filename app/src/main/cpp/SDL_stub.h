#pragma once

// Minimal SDL stub for Android - provides types and constants used by xrEngine
// Real SDL2 input handling not used on Android (handled via JNI)

#include <cstdlib>
#include <cstring>
#include <cstdio>

// SDL string functions - map to standard C
#define SDL_strlen strlen
#define SDL_strlcpy(dst, src, size) strncpy(dst, src, size)
#define SDL_free free
#define SDL_malloc malloc

// SDL scancode constants
#define SDL_NUM_SCANCODES 512
#define SDL_SCANCODE_UNKNOWN 0

// SDL controller axis count
#define SDL_CONTROLLER_AXIS_MAX 6

// SDL system cursor count
#define SDL_NUM_SYSTEM_CURSORS 11

// Forward declarations for SDL types (minimal stubs)
struct SDL_GameController;
struct SDL_Cursor;

// SDL system cursor enum
typedef enum {
    SDL_SYSTEM_CURSOR_ARROW,
    SDL_SYSTEM_CURSOR_IBEAM,
    SDL_SYSTEM_CURSOR_WAIT,
    SDL_SYSTEM_CURSOR_CROSSHAIR,
    SDL_SYSTEM_CURSOR_WAITARROW,
    SDL_SYSTEM_CURSOR_SIZENWSE,
    SDL_SYSTEM_CURSOR_SIZENESW,
    SDL_SYSTEM_CURSOR_SIZEWE,
    SDL_SYSTEM_CURSOR_SIZENS,
    SDL_SYSTEM_CURSOR_SIZEALL,
    SDL_SYSTEM_CURSOR_NO,
    SDL_SYSTEM_CURSOR_HAND
} SDL_SystemCursor;

// SDL_GetPrefPath - returns Android-specific user data directory
inline char* SDL_GetPrefPath(const char* org, const char* app) {
    const char* path = "/sdcard/Android/data/com.openxray.stalker/files/";
    char* result = (char*)malloc(strlen(path) + 1);
    if (result) {
        strcpy(result, path);
    }
    return result;
}

// SDL_PumpEvents - stub for Android (no-op)
inline void SDL_PumpEvents() {
    // No-op on Android - event handling done via JNI
}
