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

// Simple test rendering - colored triangle
static GLuint g_testShaderProgram = 0;
static GLuint g_testVAO = 0;
static GLuint g_testVBO = 0;

static const char* g_testVertexShader = R"(
#version 300 es
precision highp float;

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vColor;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    vColor = aColor;
}
)";

static const char* g_testFragmentShader = R"(
#version 300 es
precision highp float;

in vec3 vColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vColor, 1.0);
}
)";

static bool InitTestRenderer() {
    LOGI("Initializing test renderer...");
    
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &g_testVertexShader, nullptr);
    glCompileShader(vertexShader);
    
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        LOGE("Vertex shader compilation failed: %s", infoLog);
        return false;
    }
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &g_testFragmentShader, nullptr);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        LOGE("Fragment shader compilation failed: %s", infoLog);
        return false;
    }
    
    // Link shader program
    g_testShaderProgram = glCreateProgram();
    glAttachShader(g_testShaderProgram, vertexShader);
    glAttachShader(g_testShaderProgram, fragmentShader);
    glLinkProgram(g_testShaderProgram);
    
    glGetProgramiv(g_testShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(g_testShaderProgram, 512, nullptr, infoLog);
        LOGE("Shader program linking failed: %s", infoLog);
        return false;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Create triangle vertices (position + color)
    float vertices[] = {
        // Position      // Color (R, G, B)
         0.0f,  0.5f,    1.0f, 0.0f, 0.0f,  // Top (Red)
        -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,  // Bottom-left (Green)
         0.5f, -0.5f,    0.0f, 0.0f, 1.0f   // Bottom-right (Blue)
    };
    
    glGenVertexArrays(1, &g_testVAO);
    glGenBuffers(1, &g_testVBO);
    
    glBindVertexArray(g_testVAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_testVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    LOGI("✓ Test renderer initialized successfully");
    return true;
}

static void RenderTestTriangle() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(g_testShaderProgram);
    glBindVertexArray(g_testVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

static void DestroyTestRenderer() {
    if (g_testVAO) {
        glDeleteVertexArrays(1, &g_testVAO);
        g_testVAO = 0;
    }
    if (g_testVBO) {
        glDeleteBuffers(1, &g_testVBO);
        g_testVBO = 0;
    }
    if (g_testShaderProgram) {
        glDeleteProgram(g_testShaderProgram);
        g_testShaderProgram = 0;
    }
}

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
        LOGI("Step 1: Preparing to initialize xrCore...");
        
        // Set working directory to external storage path
        // xrCore may call getcwd() internally which returns nullptr on Android without this
        if (chdir(externalStoragePath) != 0) {
            LOGE("Failed to chdir to %s: %s", externalStoragePath, strerror(errno));
        } else {
            LOGI("Changed working directory to: %s", externalStoragePath);
        }
        
        // Set environment variables that xrCore might expect
        // On Android these are often nullptr which causes strncpy() crashes
        setenv("HOME", dataPath, 1);
        setenv("USER", "android", 1);
        setenv("TMPDIR", dataPath, 1);
        setenv("TEMP", dataPath, 1);
        LOGI("Set environment variables: HOME=%s, USER=android, TMPDIR=%s", dataPath, dataPath);
        
        // Initialize xrCore with command line
        // Core.Initialize signature: (ApplicationName, commandLine, init_fs, fs_fname, plugin)
        // Build command line: -nointro -noprefetch
        std::string cmdLine = "-nointro -noprefetch";
        
        LOGI("Step 2: Calling Core.Initialize()...");
        LOGI("  App name: xr_3da");
        LOGI("  Command line: %s", cmdLine.c_str());
        LOGI("  Init FS: true");
        LOGI("  FS filename: %s", fsGameLtx.c_str());
        LOGI("  Plugin: false");
        
        // Pass fsgame.ltx path to Core.Initialize
        // xrCore needs this to set up virtual file system paths
        Core.Initialize("xr_3da", cmdLine.c_str(), true, fsGameLtx.c_str(), false);

        LOGI("Step 3: xrCore initialized successfully!");

        // NOTE: Engine and game systems temporarily disabled
        // They require GEnv, Device, render subsystem, and other dependencies
        // that aren't available yet in this minimal Android port
        // Engine.Initialize(game, renderers); // Needs GameModule and RendererModule
        // Level initialization also skipped for now
        
        // Initialize test renderer (simple colored triangle)
        if (!InitTestRenderer()) {
            LOGE("Failed to initialize test renderer");
            return -3;
        }

        g_engineInitialized = true;
        LOGI("Step 4: Initialization complete, returning success");
        return 0;

    } catch (const std::exception& e) {
        LOGE("✗ Exception during xrCore initialization: %s", e.what());
        return -1;
    } catch (...) {
        LOGE("✗ Unknown exception during xrCore initialization");
        return -2;
    }
}

void android_xray_destroy() {
    LOGI("=== OpenXRay Shutdown ===");

    if (!g_engineInitialized) {
        return;
    }

    try {
        // Destroy test renderer
        DestroyTestRenderer();
        
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

        // Render test triangle (colorful gradient triangle)
        RenderTestTriangle();

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
