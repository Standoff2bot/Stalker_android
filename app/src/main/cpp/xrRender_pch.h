#pragma once

// Precompiled header for xrRender module
// Force-included for all xrRender source files to provide missing types

// Basic xrCore types
#include "xrCore/xrCore.h"
#include "xrCore/_sphere.h"
#include "xrCore/_fbox.h"
#include "xrCore/_vector3d.h"
#include "xrCore/_matrix.h"
#include "xrCore/_color.h"
#include "xrCore/_flags.h"

// Global environment (GEnv) - must be before xrEngine/Render.h
#include "Include/xrAPI/xrAPI.h"

// Rendering types from xrEngine
#include "xrEngine/Render.h"

// Define ref_shader, ref_geom, ref_constant if not available
// These are smart pointer types for render resources
#ifndef ref_shader
namespace xray::render::RENDER_NAMESPACE {
    class CBlender;
    template<typename T> class intrusive_ptr;
    // Placeholder definitions - will be replaced by actual render backend
    using ref_shader = void*;
    using ref_geom = void*;
    using ref_constant = void*;
}
#endif
