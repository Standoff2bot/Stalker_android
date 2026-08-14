#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <string>
#include <cstring>

#define LOG_TAG "OpenXRay"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// OpenXRay engine wrapper functions that bridge to the actual engine code

static std::string g_fs_root;
static std::string g_fs_fname;
static bool g_engine_running = false;

extern "C" {

// These will be implemented by linking with OpenXRay's actual engine code
// For now, provide stubs that will be replaced with real implementations

int xrEngine_Initialize(const char* fs_root, const char* fs_fname) {
    LOGI("xrEngine_Initialize called");
    LOGI("  fs_root: %s", fs_root);
    LOGI("  fs_fname: %s", fs_fname);
    
    g_fs_root = fs_root;
    g_fs_fname = fs_fname;
    
    // TODO: Call actual OpenXRay Core_Initialize()
    // For now, just initialize OpenGL ES context
    
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    LOGI("OpenGL ES Info:");
    LOGI("  Vendor: %s", vendor);
    LOGI("  Renderer: %s", renderer);
    LOGI("  Version: %s", version);
    LOGI("  GLSL Version: %s", glsl_version);
    
    g_engine_running = true;
    return 0;
}

void xrEngine_Destroy() {
    LOGI("xrEngine_Destroy called");
    // TODO: Call actual OpenXRay engine shutdown
    g_engine_running = false;
}

void xrEngine_OnFrame() {
    if (!g_engine_running) return;
    
    // TODO: Call actual OpenXRay render frame
    // For now, just clear screen with dark green (STALKER style)
    glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void xrEngine_SetViewport(int width, int height) {
    LOGI("xrEngine_SetViewport: %dx%d", width, height);
    // TODO: Update OpenXRay engine viewport
    glViewport(0, 0, width, height);
}

void xrEngine_Pause() {
    LOGI("xrEngine_Pause called");
    // TODO: Pause game logic
}

void xrEngine_Resume() {
    LOGI("xrEngine_Resume called");
    // TODO: Resume game logic
}

} // extern "C"
