#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <string>

#define LOG_TAG "OpenXRay"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Forward declarations from android_main.cpp
extern "C" {
    int android_xray_init(const char* dataPath, const char* externalStoragePath);
    void android_xray_destroy();
    void android_xray_set_viewport(int width, int height);
    void android_xray_on_frame();
    void android_xray_pause();
    void android_xray_resume();
}

static std::string g_internalDataPath;
static std::string g_externalStoragePath;
static bool g_initialized = false;

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeInit(JNIEnv* env, jobject /* this */, jstring internalPath, jstring externalPath) {
    const char* internal = env->GetStringUTFChars(internalPath, nullptr);
    const char* external = env->GetStringUTFChars(externalPath, nullptr);

    g_internalDataPath = internal;
    g_externalStoragePath = external;

    env->ReleaseStringUTFChars(internalPath, internal);
    env->ReleaseStringUTFChars(externalPath, external);

    LOGI("Native init:");
    LOGI("  Internal: %s", g_internalDataPath.c_str());
    LOGI("  External: %s", g_externalStoragePath.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeSurfaceCreated(JNIEnv*, jobject /* this */) {
    LOGI("=== Surface Created ===");

    if (!g_initialized) {
        LOGI("Initializing OpenXRay engine...");

        int result = android_xray_init(
            g_internalDataPath.c_str(),
            g_externalStoragePath.c_str()
        );

        if (result == 0) {
            g_initialized = true;
            LOGI("✓ OpenXRay engine initialized successfully");
        } else {
            LOGE("✗ Failed to initialize OpenXRay engine: error %d", result);
        }
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeSurfaceChanged(JNIEnv*, jobject /* this */, jint width, jint height) {
    LOGI("Surface changed: %dx%d", width, height);

    if (g_initialized) {
        android_xray_set_viewport(width, height);
    }

    glViewport(0, 0, width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeDrawFrame(JNIEnv*, jobject /* this */) {
    android_xray_on_frame();
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativePause(JNIEnv*, jobject /* this */) {
    LOGI("=== Application Paused ===");
    if (g_initialized) {
        android_xray_pause();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeResume(JNIEnv*, jobject /* this */) {
    LOGI("=== Application Resumed ===");
    if (g_initialized) {
        android_xray_resume();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeDestroy(JNIEnv*, jobject /* this */) {
    LOGI("=== Application Destroyed ===");
    if (g_initialized) {
        android_xray_destroy();
        g_initialized = false;
    }
}
