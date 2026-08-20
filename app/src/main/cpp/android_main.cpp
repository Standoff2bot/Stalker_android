// MUST be first: defines platform macros (XRCORE_API, IC, ICF, etc.)
#include "Common/Platform.hpp"

#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <string>
#include <cstring>

// OpenXRay engine headers
#include "xrEngine/Engine.h"
#include "xrCore/xrCore.h"
#include "xrGame/Level.h"

#define LOG_TAG "OpenXRay"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

static std::string g_dataPath;
static std::string g_externalStoragePath;
static bool g_engineInitialized = false;
static int g_viewportWidth = 0;
static int g_viewportHeight = 0;

extern "C" {

// Initialize OpenXRay engine with Android paths
int android_xray_init(const char* dataPath, const char* externalStoragePath) {
    LOGI("=== OpenXRay Android Initialization ===");
    LOGI("Data path: %s", dataPath);
    LOGI("External storage: %s", externalStoragePath);
    
    g_dataPath = dataPath;
    g_externalStoragePath = externalStoragePath;
    
    // Set up file system root
    // Android apps store game data in: /sdcard/Android/data/com.openxray.stalker/files/
    std::string fsRoot = externalStoragePath;
    std::string fsGameLtx = fsRoot + "/fsgame.ltx";
    
    LOGI("FS Root: %s", fsRoot.c_str());
    LOGI("fsgame.ltx: %s", fsGameLtx.c_str());
    
    // Check OpenGL ES info
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    LOGI("OpenGL ES Info:");
    LOGI("  Vendor: %s", vendor ? (const char*)vendor : "NULL");
    LOGI("  Renderer: %s", renderer ? (const char*)renderer : "NULL");
    LOGI("  Version: %s", version ? (const char*)version : "NULL");
    LOGI("  GLSL: %s", glsl ? (const char*)glsl : "NULL");
    
    try {
        // Initialize xrCore
        Core._initialize("xrEngine", nullptr);
        
        // Set command line parameters
        // -nointro: skip intro videos
        // -noprefetch: disable prefetch (may help on mobile)
        char* argv[] = {
            (char*)"xr_3da",
            (char*)"-nointro",
            (char*)"-noprefetch"
        };
        Core.Initialize(3, argv);
        
        LOGI("xrCore initialized successfully");
        
        // Initialize engine
        Engine.Initialize();
        Engine.External.Initialize();
        
        LOGI("OpenXRay Engine initialized successfully");
        
        g_engineInitialized = true;
        return 0;
        
    } catch (const std::exception& e) {
        LOGE("Exception during engine initialization: %s", e.what());
        return -1;
    } catch (...) {
        LOGE("Unknown exception during engine initialization");
        return -2;
    }
}

void android_xray_destroy() {
    LOGI("=== OpenXRay Shutdown ===");
    
    if (!g_engineInitialized) {
        return;
    }
    
    try {
        Engine.Destroy();
        Core._destroy();
        
        LOGI("OpenXRay Engine destroyed successfully");
        
    } catch (const std::exception& e) {
        LOGE("Exception during engine shutdown: %s", e.what());
    } catch (...) {
        LOGE("Unknown exception during engine shutdown");
    }
    
    g_engineInitialized = false;
}

void android_xray_set_viewport(int width, int height) {
    LOGI("Viewport changed: %dx%d", width, height);
    g_viewportWidth = width;
    g_viewportHeight = height;
    
    if (g_engineInitialized) {
        // Update engine viewport
        // TODO: Call appropriate OpenXRay method to resize viewport
    }
}

void android_xray_on_frame() {
    if (!g_engineInitialized) {
        // Clear screen while not initialized
        glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return;
    }
    
    try {
        // Main engine frame update
        Engine.OnFrame();
        
    } catch (const std::exception& e) {
        LOGE("Exception during frame rendering: %s", e.what());
    } catch (...) {
        LOGE("Unknown exception during frame rendering");
    }
}

void android_xray_pause() {
    LOGI("Application paused");
    
    if (g_engineInitialized) {
        // Pause game logic
        // TODO: Implement proper pause handling
    }
}

void android_xray_resume() {
    LOGI("Application resumed");
    
    if (g_engineInitialized) {
        // Resume game logic
        // TODO: Implement proper resume handling
    }
}

} // extern "C"
