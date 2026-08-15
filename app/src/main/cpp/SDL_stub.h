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

// SDL MessageBox types and constants
typedef enum {
    SDL_MESSAGEBOX_ERROR = 0x00000010,
    SDL_MESSAGEBOX_WARNING = 0x00000020,
    SDL_MESSAGEBOX_INFORMATION = 0x00000040
} SDL_MessageBoxFlags;

typedef enum {
    SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT = 0x00000001,
    SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT = 0x00000002
} SDL_MessageBoxButtonFlags;

typedef struct SDL_MessageBoxButtonData {
    Uint32 flags;
    int buttonid;
    const char* text;
} SDL_MessageBoxButtonData;

typedef struct SDL_MessageBoxData {
    Uint32 flags;
    SDL_Window* window;
    const char* title;
    const char* message;
    int numbuttons;
    const SDL_MessageBoxButtonData* buttons;
    const void* colorScheme;
} SDL_MessageBoxData;

// SDL Assertion types and constants
typedef enum {
    SDL_ASSERTION_RETRY = 0,
    SDL_ASSERTION_BREAK = 1,
    SDL_ASSERTION_ABORT = 2,
    SDL_ASSERTION_IGNORE = 3,
    SDL_ASSERTION_ALWAYS_IGNORE = 4
} SDL_AssertState;

typedef struct SDL_AssertData {
    int always_ignore;
    unsigned int trigger_count;
    const char* condition;
    const char* filename;
    int linenum;
    const char* function;
    const struct SDL_AssertData* next;
} SDL_AssertData;

typedef SDL_AssertState (*SDL_AssertionHandler)(const SDL_AssertData* data, void* userdata);

// SDL Log priority
typedef enum {
    SDL_LOG_CATEGORY_APPLICATION = 0,
    SDL_LOG_CATEGORY_ERROR = 1,
    SDL_LOG_CATEGORY_ASSERT = 2,
    SDL_LOG_CATEGORY_SYSTEM = 3,
    SDL_LOG_CATEGORY_AUDIO = 4,
    SDL_LOG_CATEGORY_VIDEO = 5,
    SDL_LOG_CATEGORY_RENDER = 6,
    SDL_LOG_CATEGORY_INPUT = 7,
    SDL_LOG_CATEGORY_TEST = 8,
    SDL_LOG_CATEGORY_RESERVED1 = 9,
    SDL_LOG_CATEGORY_RESERVED2 = 10,
    SDL_LOG_CATEGORY_RESERVED3 = 11,
    SDL_LOG_CATEGORY_RESERVED4 = 12,
    SDL_LOG_CATEGORY_RESERVED5 = 13,
    SDL_LOG_CATEGORY_RESERVED6 = 14,
    SDL_LOG_CATEGORY_RESERVED7 = 15,
    SDL_LOG_CATEGORY_RESERVED8 = 16,
    SDL_LOG_CATEGORY_RESERVED9 = 17,
    SDL_LOG_CATEGORY_RESERVED10 = 18,
    SDL_LOG_CATEGORY_CUSTOM = 19
} SDL_LogCategory;

typedef enum {
    SDL_LOG_PRIORITY_VERBOSE = 1,
    SDL_LOG_PRIORITY_DEBUG = 2,
    SDL_LOG_PRIORITY_INFO = 3,
    SDL_LOG_PRIORITY_WARN = 4,
    SDL_LOG_PRIORITY_ERROR = 5,
    SDL_LOG_PRIORITY_CRITICAL = 6,
    SDL_NUM_LOG_PRIORITIES = 7
} SDL_LogPriority;

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

// SDL_GetError - stub for Android (returns empty string)
inline const char* SDL_GetError() {
    return "";
}

// SDL_HasClipboardText - stub for Android (returns false)
inline bool SDL_HasClipboardText() {
    return false;
}

// SDL_GetClipboardText - stub for Android (returns empty string)
inline char* SDL_GetClipboardText() {
    char* result = (char*)malloc(1);
    if (result) result[0] = '\0';
    return result;
}

// SDL_SetClipboardText - stub for Android (returns 0)
inline int SDL_SetClipboardText(const char* text) {
    // No-op on Android - clipboard handled via JNI
    return 0;
}

// SDL_ShowMessageBox - stub for Android (returns 0)
inline int SDL_ShowMessageBox(const SDL_MessageBoxData* messageboxdata, int* buttonid) {
    // No-op on Android - message boxes handled via JNI
    if (buttonid) *buttonid = 0;
    return 0;
}

// SDL_SetAssertionHandler - stub for Android (no-op)
inline void SDL_SetAssertionHandler(SDL_AssertionHandler handler, void* userdata) {
    // No-op on Android - assertions handled differently
}

// SDL_GetPlatform - returns "Android"
inline const char* SDL_GetPlatform() {
    return "Android";
}

// SDL_GetBasePath - returns Android app directory
inline char* SDL_GetBasePath() {
    const char* path = "/sdcard/Android/data/com.openxray.stalker/files/";
    char* result = (char*)malloc(strlen(path) + 1);
    if (result) {
        strcpy(result, path);
    }
    return result;
}

// SDL_ShowSimpleMessageBox - stub for Android (returns 0)
inline int SDL_ShowSimpleMessageBox(Uint32 flags, const char* title, const char* message, SDL_Window* window) {
    // No-op on Android - message boxes handled via JNI
    return 0;
}

// SDL_arraysize - macro for array size
#define SDL_arraysize(array) (sizeof(array)/sizeof(array[0]))

// SDL_LogSetOutputFunction - stub for Android (no-op)
typedef void (*SDL_LogOutputFunction)(void* userdata, int category, SDL_LogPriority priority, const char* message);
inline void SDL_LogSetOutputFunction(SDL_LogOutputFunction callback, void* userdata) {
    // No-op on Android - logging handled differently
}
