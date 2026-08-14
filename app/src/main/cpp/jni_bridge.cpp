#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <string>

#define LOG_TAG "OpenXRay"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Forward declarations of OpenXRay engine functions
extern "C" {
    int xrEngine_Initialize(const char* fs_root, const char* fs_fname);
    void xrEngine_Destroy();
    void xrEngine_OnFrame();
    void xrEngine_SetViewport(int width, int height);
    void xrEngine_Pause();
    void xrEngine_Resume();
}

static std::string g_dataPath;
static bool g_initialized = false;

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeInit(JNIEnv* env, jclass, jstring dataPath) {
    const char* path = env->GetStringUTFChars(dataPath, nullptr);
    g_dataPath = path;
    env->ReleaseStringUTFChars(dataPath, path);
    
    LOGI("Native init with data path: %s", g_dataPath.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeSurfaceCreated(JNIEnv*, jclass) {
    LOGI("Surface created");
    
    if (!g_initialized) {
        // Initialize OpenXRay engine
        std::string fs_root = g_dataPath + "/";
        std::string fs_fname = g_dataPath + "/fsgame.ltx";
        
        LOGI("Initializing OpenXRay engine...");
        LOGI("FS root: %s", fs_root.c_str());
        LOGI("FS config: %s", fs_fname.c_str());
        
        int result = xrEngine_Initialize(fs_root.c_str(), fs_fname.c_str());
        if (result == 0) {
            g_initialized = true;
            LOGI("OpenXRay engine initialized successfully");
        } else {
            LOGE("Failed to initialize OpenXRay engine: %d", result);
        }
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeSurfaceChanged(JNIEnv*, jclass, jint width, jint height) {
    LOGI("Surface changed: %dx%d", width, height);
    
    if (g_initialized) {
        xrEngine_SetViewport(width, height);
    }
    
    glViewport(0, 0, width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeDrawFrame(JNIEnv*, jclass) {
    if (g_initialized) {
        xrEngine_OnFrame();
    } else {
        // Draw black screen while not initialized
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativePause(JNIEnv*, jclass) {
    LOGI("Pause");
    if (g_initialized) {
        xrEngine_Pause();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeResume(JNIEnv*, jclass) {
    LOGI("Resume");
    if (g_initialized) {
        xrEngine_Resume();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeDestroy(JNIEnv*, jclass) {
    LOGI("Destroy");
    if (g_initialized) {
        xrEngine_Destroy();
        g_initialized = false;
    }
}
