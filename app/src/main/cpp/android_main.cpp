#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <string>
#include <cstring>

// OpenXRay engine headers
// NOTE: xrCore.h MUST be included first - it includes Platform.hpp which defines
// platform macros (XRCORE_API, IC, ICF, XR_EXPORT, XR_IMPORT, etc.)
// Other headers (Engine.h, Level.h) depend on these macros being defined
#include "xrCore/xrCore.h"
// Engine and Level temporarily disabled - they require GEnv, Device, and render subsystem
// #include "xrEngine/Engine.h"
// #include "xrGame/Level.h"

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
        // Initialize xrCore with command line
        // Core.Initialize signature: (ApplicationName, commandLine, init_fs, fs_fname, plugin)
        // Build command line: -nointro -noprefetch
        std::string cmdLine = "-nointro -noprefetch";
        Core.Initialize("xr_3da", cmdLine.c_str(), true, nullptr, false);

        LOGI("xrCore initialized successfully");

        // NOTE: Engine and game systems temporarily disabled
        // They require GEnv, Device, render subsystem, and other dependencies
        // that aren't available yet in this minimal Android port
        // Engine.Initialize(game, renderers); // Needs GameModule and RendererModule
        // Level initialization also skipped for now

        g_engineInitialized = true;
        return 0;

    } catch (const std::exception& e) {
        LOGE("Exception during xrCore initialization: %s", e.what());
        return -1;
    } catch (...) {
        LOGE("Unknown exception during xrCore initialization");
        return -2;
    }
}

void android_xray_destroy() {
    LOGI("=== OpenXRay Shutdown ===");

    if (!g_engineInitialized) {
        return;
    }

    try {
        // Engine.Destroy(); // Disabled - Engine not initialized
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
        // Main engine frame update - disabled (Engine not initialized)
        // Engine.OnFrame();
        
        // For now, just clear the screen
        glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
