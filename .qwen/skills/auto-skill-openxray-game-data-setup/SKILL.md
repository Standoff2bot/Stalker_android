---
name: openxray-game-data-setup
description: Configure STALKER game data for OpenXRay engine, understanding .db archives vs unpacked gamedata requirements and shader setup
source: auto-skill
extracted_at: '2026-08-14T18:10:17.207Z'
---

# OpenXRay Game Data Setup for STALKER Games

This skill captures the non-obvious approach for setting up game data files when running OpenXRay engine with STALKER games (Call of Pripyat, Clear Sky, Shadow of Chernobyl). The key insight is understanding what OpenXRay needs vs what the original game provides.

## When to Use

- Setting up OpenXRay for the first time with STALKER game files
- Encountering "Can't open section" or "Can't open shader" errors
- Debugging missing gameplay XML files (info_*.xml)
- Understanding why OpenXRay requires `gamedata/` folder when original game doesn't have one
- Configuring OpenGL ES renderer on non-Windows platforms

## Core Understanding: .db Archives vs gamedata/

### Original STALKER Games

**Do NOT use unpacked `gamedata/` folder.** All game content is packed in `.db` archives:

```
STALKER/
├── bin/                    # Game executables
├── fsgame.ltx              # File system configuration
├── levels/                 # Level files
├── localization/           # Localization
├── patches/                # Patches
└── resources/              # ← CRITICAL: Contains all game data
    ├── configs.db          # 42 MB - All gameplay configs (XML, LTX)
    ├── resources.db0       # 641 MB - Textures, models (part 1)
    ├── resources.db1       # 641 MB - (part 2)
    ├── resources.db2       # 646 MB - (part 3)
    ├── resources.db3       # 641 MB - (part 4)
    └── resources.db4       # 240 MB - (part 5)
```

**Total:** ~2.8 GB of packed game data.

### What OpenXRay Needs

OpenXRay **reads configs and resources directly from .db archives** (no unpacking needed for COP), BUT requires:

1. **For Call of Pripyat:** Only OpenGL shaders in `gamedata/shaders/gl/` (if using OpenGL renderer)
2. **For Clear Sky:** Complete unpacked `gamedata/` folder (official gamedata.zip from OpenXRay wiki)
3. **For Call of Chernobyl:** Complete unpacked `gamedata/` folder (official gamedata.zip)

## Common Mistake: Incomplete gamedata/ Installations

### Problem Pattern

You may find incomplete game installations that contain:
- `gamedata/configs/` with only UI configs (`system.ltx`, `localization.ltx`)
- `gamedata/shaders/gl/` with OpenGL shaders
- `gamedata/scripts/` with Lua scripts
- **Missing:** `gamedata/configs/gameplay/`, `gamedata/configs/creatures/`, `gamedata/configs/items/`

These are **mod-only installations** (UI mods, shader packs, script mods) that assume you have the original game's `.db` archives.

### Error Symptoms

```
[error] Description : Can't open section 'stat_font'
[error] Arguments   : (only 'string_table' avail)
```

or

```
[error] Description : Error reading Attributes.
[error] Arguments   : gameplay\info_jupiter.xml
```

**Root cause:** Incomplete `gamedata/configs/system.ltx` overrides the complete one inside `configs.db`, or missing gameplay files.

## Correct Setup for Call of Pripyat + OpenXRay

### Directory Structure

```
~/.local/share/GSC Game World/S.T.A.L.K.E.R. - Call of Pripyat/
├── fsgame.ltx          → symlink to original game
├── levels/             → symlink to original game
├── localization/       → symlink to original game  
├── resources/          → symlink to original game (.db archives)
├── patches/            → symlink to original game
└── gamedata/           ← ONLY for OpenGL shaders (if using GL renderer)
    └── shaders/
        └── gl/         ← OpenGL ES shader sources
            ├── iostructs/
            ├── shared/
            └── *.vs, *.gs, *.ps files
```

**Critical:** Do NOT include incomplete `gamedata/configs/` - let OpenXRay read from `resources/configs.db`.

### Setup Commands

```bash
GAME_DIR="$HOME/.local/share/GSC Game World/S.T.A.L.K.E.R. - Call of Pripyat"
STALKER_INSTALL="$HOME/storage/downloads/STALKER/S.T.A.L.K.E.R. Call of Pripyat"

mkdir -p "$GAME_DIR"
cd "$GAME_DIR"

# Link to original game files (contains .db archives)
ln -sf "$STALKER_INSTALL/fsgame.ltx" fsgame.ltx
ln -sf "$STALKER_INSTALL/levels" levels
ln -sf "$STALKER_INSTALL/localization" localization
ln -sf "$STALKER_INSTALL/resources" resources
ln -sf "$STALKER_INSTALL/patches" patches

# ONLY add OpenGL shaders if you have them
# (from OpenXRay shader pack or mod with GL shaders)
mkdir -p gamedata/shaders
cp -r /path/to/gl_shaders gamedata/shaders/gl
```

## OpenGL Shader Requirements

### Why Shaders are Separate

Original STALKER uses **DirectX shaders** (HLSL) for Windows. OpenXRay's OpenGL renderer needs **GLSL shaders**, which are:

1. Not included in original game `.db` archives
2. Distributed separately by OpenXRay project
3. Often bundled with OpenXRay-compatible mods

### Where to Get OpenGL Shaders

**Option 1:** OpenXRay GitHub repository
- Path: `res/gamedata/shaders/gl/`
- Clone and copy: `git clone https://github.com/OpenXRay/xray-16.git && cp -r xray-16/res/gamedata/shaders/gl ~/.local/share/.../gamedata/shaders/`

**Option 2:** OpenXRay-compatible mods
- Many mods (Gunslinger, Anomaly port) include updated GL shaders
- Check mod's `gamedata/shaders/gl/` directory

**Option 3:** Existing OpenXRay installation
- If you have OpenXRay working on Windows/Linux, copy shaders from there

### Shader Structure

```
gamedata/shaders/gl/
├── iostructs/          # Input/output structures
│   ├── v_shadow.h
│   ├── v_static.h
│   └── ...
├── shared/             # Shared functions
│   ├── common.h
│   ├── lighting.h
│   └── ...
├── skin.h              # ← Must exist! Skinned mesh shader
├── accum_*.vs/gs/ps    # Accumulation passes
├── combine_*.vs/gs/ps  # Combine passes
├── deffer_*.vs/gs/ps   # Deferred shading
└── ... (200+ shader files)
```

**Critical file:** `skin.h` - If this is missing, you'll get:
```
[error] Description : Can't open shader
[error] Arguments   : skin.h
```

## Verification After Setup

### 1. Check File System Cache

Run OpenXRay once and look for:
```
FS: 39453 files cached 12 archives, 51Kb memory used.
```

- **~39000 files:** Good, all .db archives loaded
- **12 archives:** Correct for Call of Pripyat (levels + resources)
- If you see fewer files (e.g., 5000), archives aren't being read

### 2. Verify Shader Loading

Should see:
```
Processing 2736 .thm files in [$game_textures$]
Load, texture descriptions loaded for 982 ms
* HWDST/PCF supported and used
```

If you get `Can't open shader` before this, shaders are missing.

### 3. Check Script Loading

Should see scripts loading:
```
* Loading script: _G.script
* Loading script: class_registrator.script
* Loading script: game_registrator.script
* Loading script: smart_terrain.script
...
```

If scripts fail to load, `configs.db` isn't being read or LuaJIT has issues.

## Troubleshooting: Multiple Game Installations

### Problem

You may have multiple STALKER installations:
1. Original game install (packed .db archives) ✅
2. Multiplayer-only version (no gameplay configs) ❌
3. Mod installation (shaders + UI only) ❌

### How to Identify Complete Installation

```bash
# Check for .db archives (must have configs.db and resources.db*)
ls -lh "$STALKER_DIR/resources/"
# Should show: configs.db (~42 MB) + resources.db0-db4 (2.8 GB total)

# Verify configs.db exists and is not empty
file "$STALKER_DIR/resources/configs.db"
# Should show: data (not empty text file)

# Check it's not a multiplayer-only version
ls "$STALKER_DIR/"
# Should have: bin/, levels/, localization/, resources/, fsgame.ltx
# Should NOT only have: mp/, bin/ (multiplayer-only)
```

## When to Use gamedata.zip from OpenXRay

For **Clear Sky** and **Call of Chernobyl**, download official gamedata.zip from OpenXRay wiki:

```bash
# Clear Sky
wget https://github.com/OpenXRay/xray-16/releases/.../gamedata_cs.zip
unzip gamedata_cs.zip -d "$GAME_DIR/"

# Call of Chernobyl  
wget https://github.com/OpenXRay/xray-16/releases/.../gamedata_coc.zip
unzip gamedata_coc.zip -d "$GAME_DIR/"
```

**For Call of Pripyat:** NOT needed, engine reads from .db archives.

## Real-World Example: The Investigation

Started with error:
```
Error reading Attributes: gameplay\info_jupiter.xml
```

Investigation revealed:
1. `gamedata/` pointed to incomplete mod installation (UI + shaders only)
2. No `gamedata/configs/gameplay/` directory existed
3. Original game had complete `resources/configs.db` with all gameplay files

**Solution:** Removed incomplete `gamedata/configs/`, kept only `gamedata/shaders/gl/`. OpenXRay then successfully read configs from `.db` archives.

## Key Takeaway

**For Call of Pripyat + OpenXRay:**
- ✅ Point resources/ to original game (with .db archives)
- ✅ Add gamedata/shaders/gl/ for OpenGL renderer
- ❌ Do NOT add incomplete gamedata/configs/ that overrides .db archives
- ❌ Do NOT unpack .db archives unless using Clear Sky/CoC

OpenXRay is designed to work with original packed game data, only requiring additional files (like OpenGL shaders) that weren't in the original game.
