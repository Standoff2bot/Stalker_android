#pragma once

// Minimal SDL stub for Android - provides types and constants used by xrEngine
// Real SDL2 input handling not used on Android (handled via JNI)

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstdint>

// SDL string functions - map to standard C
#define SDL_strlen strlen
#define SDL_strlcpy(dst, src, size) strncpy(dst, src, size)
#define SDL_free free
#define SDL_malloc malloc

// SDL basic types
typedef uint32_t Uint32;
typedef uint8_t Uint8;

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
struct SDL_Window;

// SDL rectangle structure
typedef struct SDL_Rect {
    int x, y;
    int w, h;
} SDL_Rect;

// SDL event structure (minimal stub)
typedef struct SDL_Event {
    Uint32 type;
    Uint32 timestamp;
} SDL_Event;

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

// SDL_CreateWindow - stub for Android (returns nullptr)
inline SDL_Window* SDL_CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags) {
    // No-op on Android - window management done via JNI
    return nullptr;
}

// SDL_DestroyWindow - stub for Android (no-op)
inline void SDL_DestroyWindow(SDL_Window* window) {
    // No-op on Android - window management done via JNI
}
