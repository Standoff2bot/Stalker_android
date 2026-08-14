# Game Data Installation Guide

## Where to Put Game Files

After installing the APK, you need to copy your S.T.A.L.K.E.R.: Call of Pripyat game files to your Android device.

### Installation Path

Place all game files in:
```
/sdcard/Android/data/com.openxray.stalker/files/
```

This directory will be automatically created when you first launch the app.

### Required File Structure

```
/sdcard/Android/data/com.openxray.stalker/files/
├── fsgame.ltx          # File system configuration (REQUIRED)
├── gamedata/           # OpenGL shaders and configs
│   ├── shaders/
│   │   └── gl/         # OpenGL ES shaders (REQUIRED for Android)
│   └── configs/        # Optional: custom configs
├── resources/          # Packed game resources (REQUIRED)
│   ├── configs.db
│   ├── resources.db0
│   ├── resources.db1
│   ├── resources.db2
│   ├── resources.db3
│   └── resources.db4
├── levels/             # Game levels (REQUIRED)
│   ├── l01_escape/
│   ├── l02_garbage/
│   ├── ...
│   └── jupiter/
├── localization/       # Game text and dialogue (REQUIRED)
│   └── rus.xml         # or eng.xml for English
└── patches/            # Optional patches
```

## Step-by-Step Installation

### Option 1: Using ADB (Recommended for Large Files)

1. Connect your Android device to PC via USB
2. Enable USB debugging in Developer Options
3. Open terminal/command prompt on your PC
4. Navigate to your STALKER installation folder:
   ```bash
   cd "C:\Program Files\S.T.A.L.K.E.R. Call of Pripyat"
   ```
5. Push files to device:
   ```bash
   adb push fsgame.ltx /sdcard/Android/data/com.openxray.stalker/files/
   adb push resources /sdcard/Android/data/com.openxray.stalker/files/resources/
   adb push levels /sdcard/Android/data/com.openxray.stalker/files/levels/
   adb push localization /sdcard/Android/data/com.openxray.stalker/files/localization/
   ```

### Option 2: Manual Copy via File Manager

1. Copy your STALKER installation folder to your device (USB cable or cloud storage)
2. Use a file manager app (like "Files by Google" or "Total Commander")
3. Navigate to `/sdcard/Android/data/com.openxray.stalker/files/`
4. Copy the required folders there

### Option 3: Using Total Commander Plugin

1. Install Total Commander on Android
2. Connect to your PC via WiFi or USB
3. Copy game folders directly

## Important: OpenGL Shaders

**The original game uses DirectX shaders which will NOT work on Android!**

You **MUST** use OpenGL shaders. Get them from:
- OpenXRay repository: https://github.com/OpenXRay/xray-16/tree/xd_dev/res/gamedata/shaders/gl
- Or use precompiled shaders from OpenXRay releases

Place OpenGL shaders in:
```
/sdcard/Android/data/com.openxray.stalker/files/gamedata/shaders/gl/
```

## Minimal Installation (Testing)

For initial testing, minimum required files (~2-3 GB):
1. `fsgame.ltx`
2. `resources/` folder with all .db files
3. `gamedata/shaders/gl/` folder with OpenGL shaders
4. `levels/l01_escape/` (first level)
5. `localization/rus.xml` or `eng.xml`

## Troubleshooting

### App crashes on startup
- Check logcat: `adb logcat | grep OpenXRay`
- Verify `fsgame.ltx` exists
- Ensure OpenGL shaders are present in `gamedata/shaders/gl/`

### "Cannot find shader" error
- You're using DirectX shaders instead of OpenGL
- Download OpenGL shaders from OpenXRay repository
- Place them in `gamedata/shaders/gl/` directory

### "Cannot open file" errors
- Check file permissions
- Verify paths are correct (case-sensitive!)
- Ensure all .db archives are present in `resources/`

### Black screen after loading
- OpenGL ES context may not be initialized
- Check GPU compatibility (requires OpenGL ES 3.0+)
- Try disabling advanced graphics features in settings

## File Sizes Reference

Approximate sizes for Call of Pripyat:
- `resources/*.db` - ~1.5 GB
- `levels/` - ~700 MB
- `gamedata/shaders/gl/` - ~50 MB
- `localization/` - ~10 MB

**Total required: ~2.3 GB minimum**

## Advanced: Custom Gamedata

If you want to use mods or custom gamedata:
1. Create `gamedata/` folder
2. Extract contents from `resources.db*` archives (if needed)
3. Mod files in `gamedata/` override packed resources
4. Keep `shaders/gl/` with OpenGL shaders (don't use DirectX ones!)

## Verification Commands

Check if files are correctly placed:
```bash
adb shell ls -lh /sdcard/Android/data/com.openxray.stalker/files/
adb shell ls -lh /sdcard/Android/data/com.openxray.stalker/files/resources/
adb shell ls -lh /sdcard/Android/data/com.openxray.stalker/files/gamedata/shaders/gl/
```

## Getting Help

If you encounter issues:
1. Check app logs: `adb logcat -s OpenXRay:*`
2. Open an issue on GitHub: https://github.com/Standoff2bot/Stalker_android/issues
3. Include logcat output and device specifications

---

**Note:** You must own a legal copy of S.T.A.L.K.E.R.: Call of Pripyat to play this port. Game files are NOT included with the APK.
