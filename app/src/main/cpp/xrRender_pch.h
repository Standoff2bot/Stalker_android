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

// ImGui types (ImTextureID)
#include "imgui/imgui.h"

// OpenGL ES 3.0+ types (GLuint, GLenum, etc.)
#include <GLES3/gl3.h>

// DirectX-to-OpenGL type mappings for xrRenderGL compatibility
// These types are referenced in xrRender code but defined differently in OpenGL backend
using D3DCMPFUNC = GLenum;      // Comparison functions (GL_NEVER, GL_LESS, etc.)
using D3DSTENCILOP = GLenum;    // Stencil operations (GL_KEEP, GL_REPLACE, etc.)
using D3DBLEND = GLenum;        // Blend factors (GL_ZERO, GL_ONE, GL_SRC_ALPHA, etc.)
using D3DBLENDOP = GLenum;      // Blend operations (GL_FUNC_ADD, GL_FUNC_SUBTRACT, etc.)
using D3DCULL = GLenum;         // Culling modes (GL_FRONT, GL_BACK, GL_NONE)
using D3DFORMAT = GLenum;       // Texture formats (GL_RGBA8, GL_DEPTH_COMPONENT24, etc.)

// DirectX blend mode constants mapped to OpenGL equivalents
#define D3DBLEND_ZERO           GL_ZERO
#define D3DBLEND_ONE            GL_ONE
#define D3DBLEND_SRCCOLOR       GL_SRC_COLOR
#define D3DBLEND_INVSRCCOLOR    GL_ONE_MINUS_SRC_COLOR
#define D3DBLEND_SRCALPHA       GL_SRC_ALPHA
#define D3DBLEND_INVSRCALPHA    GL_ONE_MINUS_SRC_ALPHA
#define D3DBLEND_DESTALPHA      GL_DST_ALPHA
#define D3DBLEND_INVDESTALPHA   GL_ONE_MINUS_DST_ALPHA
#define D3DBLEND_DESTCOLOR      GL_DST_COLOR
#define D3DBLEND_INVDESTCOLOR   GL_ONE_MINUS_DST_COLOR
#define D3DBLEND_SRCALPHASAT    GL_SRC_ALPHA_SATURATE

// DirectX texture addressing modes mapped to OpenGL
#define D3DTADDRESS_WRAP        GL_REPEAT
#define D3DTADDRESS_MIRROR      GL_MIRRORED_REPEAT
#define D3DTADDRESS_CLAMP       GL_CLAMP_TO_EDGE
#define D3DTADDRESS_BORDER      GL_CLAMP_TO_BORDER

// DirectX texture filter modes mapped to OpenGL
#define D3DTEXF_NONE            GL_NEAREST
#define D3DTEXF_POINT           GL_NEAREST
#define D3DTEXF_LINEAR          GL_LINEAR
#define D3DTEXF_ANISOTROPIC     GL_LINEAR  // Anisotropic filtering set separately

// DirectX comparison functions mapped to OpenGL
#define D3DCMP_NEVER            GL_NEVER
#define D3DCMP_LESS             GL_LESS
#define D3DCMP_EQUAL            GL_EQUAL
#define D3DCMP_LESSEQUAL        GL_LEQUAL
#define D3DCMP_GREATER          GL_GREATER
#define D3DCMP_NOTEQUAL         GL_NOTEQUAL
#define D3DCMP_GREATEREQUAL     GL_GEQUAL
#define D3DCMP_ALWAYS           GL_ALWAYS

// DirectX stencil operations mapped to OpenGL
#define D3DSTENCILOP_KEEP       GL_KEEP
#define D3DSTENCILOP_ZERO       GL_ZERO
#define D3DSTENCILOP_REPLACE    GL_REPLACE
#define D3DSTENCILOP_INCRSAT    GL_INCR
#define D3DSTENCILOP_DECRSAT    GL_DECR
#define D3DSTENCILOP_INVERT     GL_INVERT
#define D3DSTENCILOP_INCR       GL_INCR_WRAP
#define D3DSTENCILOP_DECR       GL_DECR_WRAP

// DirectX texture stage state (legacy fixed-function pipeline)
// OpenGL ES 3.0+ uses shaders, these are for compatibility only
#define D3DTSS_COLOROP          0
#define D3DTSS_COLORARG1        1
#define D3DTSS_COLORARG2        2
#define D3DTSS_ALPHAOP          3
#define D3DTSS_ALPHAARG1        4
#define D3DTSS_ALPHAARG2        5
#define D3DTSS_COLORARG0        6
#define D3DTSS_ALPHAARG0        7

// DirectX texture operation (legacy fixed-function)
#define D3DTOP_DISABLE          0
#define D3DTOP_SELECTARG1       1
#define D3DTOP_SELECTARG2       2
#define D3DTOP_MODULATE         3
#define D3DTOP_ADD              4

// Render state object (DirectX ID3D11SamplerState equivalent)
// In OpenGL we don't have state objects, just direct state setting
using ID3DState = void*;        // Placeholder - OpenGL doesn't use state objects

// Vertex buffer/index buffer handles (OpenGL uses GLuint for buffer objects)
using VertexBufferHandle = GLuint;
using IndexBufferHandle = GLuint;

// Vertex element descriptor (DirectX D3D11_INPUT_ELEMENT_DESC equivalent)
// OpenGL ES 3.0+ uses vertex attribute pointers set via glVertexAttribPointer
struct VertexElement {
    u32 Stream;           // Vertex buffer binding index
    u32 Offset;           // Byte offset from start of vertex
    u32 Type;             // Data type (GL_FLOAT, GL_INT, etc.)
    u32 Method;           // Unused in OpenGL (DirectX vertex processing method)
    u32 Usage;            // Semantic usage (position, normal, texcoord, etc.)
    u32 UsageIndex;       // Semantic index for multiple attributes of same type
};

// Rendering types from xrEngine
#include "xrEngine/Render.h"

// Shader resource pointers (ref_shader, ref_geom, ref_constant)
// Include actual definitions from xrRender, not void* placeholders
#include "Layers/xrRender/Shader.h"
#include "Layers/xrRender/Blender_Recorder.h"  // CBlender_Compile definition
