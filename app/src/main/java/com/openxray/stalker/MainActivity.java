package com.openxray.stalker;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Environment;
import android.view.WindowManager;
import android.util.Log;
import java.io.File;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends Activity {
    private static final String TAG = "OpenXRay";
    private GLSurfaceView glSurfaceView;

    static {
        System.loadLibrary("xray-engine");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setEGLContextClientVersion(3);
        glSurfaceView.setRenderer(new XRayRenderer());
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);

        setContentView(glSurfaceView);

        // Internal storage for app data (configs, logs)
        String internalPath = getFilesDir().getAbsolutePath();
        
        // External storage for game data (resources, levels, gamedata)
        // User should place game files in: /sdcard/Android/data/com.openxray.stalker/files/
        File externalFilesDir = getExternalFilesDir(null);
        String externalPath = externalFilesDir != null ? 
            externalFilesDir.getAbsolutePath() : 
            Environment.getExternalStorageDirectory().getAbsolutePath() + "/stalker";
        
        Log.i(TAG, "Internal path: " + internalPath);
        Log.i(TAG, "External path: " + externalPath);
        
        // Create external directory if it doesn't exist
        File externalDir = new File(externalPath);
        if (!externalDir.exists()) {
            externalDir.mkdirs();
            Log.i(TAG, "Created external directory: " + externalPath);
        }
        
        nativeInit(internalPath, externalPath);
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        glSurfaceView.onPause();
        nativePause();
    }
    
    @Override
    protected void onResume() {
        super.onResume();
        glSurfaceView.onResume();
        nativeResume();
    }
    
    @Override
    protected void onDestroy() {
        nativeDestroy();
        super.onDestroy();
    }
    
    private static class XRayRenderer implements GLSurfaceView.Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            nativeSurfaceCreated();
        }
        
        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            nativeSurfaceChanged(width, height);
        }
        
        @Override
        public void onDrawFrame(GL10 gl) {
            nativeDrawFrame();
        }
    }
    
    // Native methods
    private static native void nativeInit(String internalPath, String externalPath);
    private static native void nativeSurfaceCreated();
    private static native void nativeSurfaceChanged(int width, int height);
    private static native void nativeDrawFrame();
    private static native void nativePause();
    private static native void nativeResume();
    private static native void nativeDestroy();
}
