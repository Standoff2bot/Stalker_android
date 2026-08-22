package com.openxray.stalker;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.view.WindowManager;
import android.util.Log;
import android.widget.Toast;
import java.io.File;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends Activity {
    private static final String TAG = "OpenXRay";
    private static final int REQUEST_CODE_PICK_FOLDER = 1001;
    private GLSurfaceView glSurfaceView;
    private String internalPath;
    private String externalPath;
    private boolean engineInitialized = false;

    static {
        System.loadLibrary("xray-engine");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        // Internal storage for app data (configs, logs)
        internalPath = getFilesDir().getAbsolutePath();

        // External storage for game data (resources, levels, gamedata)
        // User should place game files in: /sdcard/Android/data/com.openxray.stalker/files/
        File externalFilesDir = getExternalFilesDir(null);
        externalPath = externalFilesDir != null ?
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

        // Check if game files exist
        if (!checkGameFiles()) {
            showGameFilesDialog();
            return;
        }

        initializeEngine();
    }

    private boolean checkGameFiles() {
        // Check for essential game files
        File gamedataDir = new File(externalPath, "gamedata");
        File resourcesDir = new File(externalPath, "resources");
        File fsgameLtx = new File(externalPath, "fsgame.ltx");

        Log.i(TAG, "Checking game files:");
        Log.i(TAG, "  gamedata: " + gamedataDir.exists());
        Log.i(TAG, "  resources: " + resourcesDir.exists());
        Log.i(TAG, "  fsgame.ltx: " + fsgameLtx.exists());

        // At least one directory or fsgame.ltx must exist
        return gamedataDir.exists() || resourcesDir.exists() || fsgameLtx.exists();
    }

    private void showGameFilesDialog() {
        new AlertDialog.Builder(this)
            .setTitle("Game Files Not Found")
            .setMessage("OpenXRay requires S.T.A.L.K.E.R. game files.\n\n" +
                "Please copy the following folders from your PC game installation:\n" +
                "• gamedata/\n" +
                "• resources/\n" +
                "• fsgame.ltx\n\n" +
                "Copy them to:\n" +
                externalPath + "\n\n" +
                "You can access this path using:\n" +
                "• File manager app\n" +
                "• USB connection (MTP)\n" +
                "• adb push command")
            .setPositiveButton("Choose Folder", (dialog, which) -> {
                // Open folder picker
                Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
                intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
                startActivityForResult(intent, REQUEST_CODE_PICK_FOLDER);
            })
            .setNegativeButton("Exit", (dialog, which) -> finish())
            .setNeutralButton("Retry", (dialog, which) -> {
                if (checkGameFiles()) {
                    initializeEngine();
                } else {
                    showGameFilesDialog();
                }
            })
            .setCancelable(false)
            .show();
    }

    private void initializeEngine() {
        glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setEGLContextClientVersion(3);
        glSurfaceView.setRenderer(new XRayRenderer());
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);

        setContentView(glSurfaceView);

        try {
            nativeInit(internalPath, externalPath);
            engineInitialized = true;
            Toast.makeText(this, "OpenXRay Engine Started", Toast.LENGTH_SHORT).show();
        } catch (Exception e) {
            Log.e(TAG, "Failed to initialize engine", e);
            new AlertDialog.Builder(this)
                .setTitle("Initialization Error")
                .setMessage("Failed to initialize OpenXRay engine:\n\n" + e.getMessage() +
                    "\n\nMake sure all game files are copied correctly.")
                .setPositiveButton("OK", (dialog, which) -> finish())
                .show();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_CODE_PICK_FOLDER && resultCode == RESULT_OK) {
            if (data != null) {
                Uri treeUri = data.getData();
                Toast.makeText(this, "Selected: " + treeUri.getPath() + 
                    "\n\nPlease copy game files manually for now.", Toast.LENGTH_LONG).show();
                // TODO: Copy files from selected folder to externalPath
            }
        }
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        if (glSurfaceView != null) {
            glSurfaceView.onPause();
        }
        if (engineInitialized) {
            nativePause();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (glSurfaceView != null) {
            glSurfaceView.onResume();
        }
        if (engineInitialized) {
            nativeResume();
        }
    }

    @Override
    protected void onDestroy() {
        if (engineInitialized) {
            nativeDestroy();
        }
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
