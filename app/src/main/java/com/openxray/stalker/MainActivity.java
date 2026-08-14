package com.openxray.stalker;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.WindowManager;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends Activity {
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
        
        String dataPath = getFilesDir().getAbsolutePath();
        nativeInit(dataPath);
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
    private static native void nativeInit(String dataPath);
    private static native void nativeSurfaceCreated();
    private static native void nativeSurfaceChanged(int width, int height);
    private static native void nativeDrawFrame();
    private static native void nativePause();
    private static native void nativeResume();
    private static native void nativeDestroy();
}
