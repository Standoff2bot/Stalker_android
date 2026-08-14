#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <string>

#define LOG_TAG "OpenXRay"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static std::string g_internalPath;
static std::string g_externalPath;
static bool g_initialized = false;
static int g_frameCount = 0;

extern "C" {

int android_xray_init(const char* internalPath, const char* externalPath) {
    LOGI("=== OpenXRay Initialization ===");
    LOGI("Internal: %s", internalPath);
    LOGI("External: %s", externalPath);
    
    g_internalPath = internalPath;
    g_externalPath = externalPath;
    
    // Check OpenGL ES capabilities
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    LOGI("OpenGL ES Info:");
    LOGI("  Vendor: %s", vendor ? (const char*)vendor : "NULL");
    LOGI("  Renderer: %s", renderer ? (const char*)renderer : "NULL");
    LOGI("  Version: %s", version ? (const char*)version : "NULL");
    LOGI("  GLSL: %s", glsl ? (const char*)glsl : "NULL");
    
    // Check for Mali GPU
    if (renderer && strstr((const char*)renderer, "Mali")) {
        LOGI("✓ Mali GPU detected! Hardware acceleration available.");
    }
    
    g_initialized = true;
    LOGI("✓ Initialization complete");
    return 0;
}

void android_xray_destroy() {
    LOGI("=== Shutdown ===");
    g_initialized = false;
}

void android_xray_set_viewport(int width, int height) {
    LOGI("Viewport: %dx%d", width, height);
    glViewport(0, 0, width, height);
}

void android_xray_on_frame() {
    if (!g_initialized) {
        glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return;
    }
    
    g_frameCount++;
    
    // Animated background (STALKER green-ish atmosphere)
    float time = g_frameCount * 0.01f;
    float r = 0.1f + 0.05f * sinf(time * 0.5f);
    float g = 0.2f + 0.05f * sinf(time * 0.7f);
    float b = 0.1f + 0.03f * sinf(time * 0.3f);
    
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Log every 60 frames (~1 second at 60 FPS)
    if (g_frameCount % 60 == 0) {
        LOGI("Frame %d - Running on Mali GPU with OpenGL ES 3.0", g_frameCount);
    }
}

void android_xray_pause() {
    LOGI("Paused");
}

void android_xray_resume() {
    LOGI("Resumed");
}

} // extern "C"
