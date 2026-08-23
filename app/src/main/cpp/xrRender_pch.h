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
// OpenGL ES 3.1+ for glProgramUniform* functions (separate shader objects)
#include <GLES3/gl31.h>

// GLAD compatibility layer for Android OpenGL ES
// Desktop OpenGL uses GLAD loader, but Android has native OpenGL ES
// Define GLAD feature flags to match desktop code
#define GLAD_GL_ARB_separate_shader_objects 1
// glProgramUniform* functions are available natively in OpenGL ES 3.1+

// DirectX-to-OpenGL type mappings for xrRenderGL compatibility
// These types are referenced in xrRender code but defined differently in OpenGL backend
using D3DCMPFUNC = GLenum;      // Comparison functions (GL_NEVER, GL_LESS, etc.)
using D3DSTENCILOP = GLenum;    // Stencil operations (GL_KEEP, GL_REPLACE, etc.)
using D3DBLEND = GLenum;        // Blend factors (GL_ZERO, GL_ONE, GL_SRC_ALPHA, etc.)
using D3DBLENDOP = GLenum;      // Blend operations (GL_FUNC_ADD, GL_FUNC_SUBTRACT, etc.)
using D3DCULL = GLenum;         // Culling modes (GL_FRONT, GL_BACK, GL_NONE)
using D3DFORMAT = GLenum;       // Texture formats (GL_RGBA8, GL_DEPTH_COMPONENT24, etc.)
using D3DPRIMITIVETYPE = GLenum; // Primitive types (GL_TRIANGLES, GL_LINES, etc.)
using D3DRENDERSTATETYPE = u32;  // Render state type (DirectX state IDs)
using D3DSAMPLERSTATETYPE = u32; // Sampler state type (DirectX sampler state IDs)

// DirectX viewport structure
struct D3D_VIEWPORT {
    float TopLeftX;
    float TopLeftY;
    float Width;
    float Height;
    float MinDepth;
    float MaxDepth;
};

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

// Buffer handle types (OpenGL uses GLuint for all buffer objects)
using ConstantBufferHandle = GLuint;
using HostBufferHandle = GLuint;
using InputElementDesc = VertexElement;  // Alias for compatibility

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

// DirectX culling modes mapped to OpenGL
#define D3DCULL_NONE            GL_NONE
#define D3DCULL_CW              0x0900  // GL_CW (not standard in ES, placeholder)
#define D3DCULL_CCW             0x0901  // GL_CCW (not standard in ES, placeholder)

// DirectX color write enable flags (mapped to OpenGL color mask)
#define D3DCOLORWRITEENABLE_RED     0x00000001  // GL_TRUE for red channel
#define D3DCOLORWRITEENABLE_GREEN   0x00000002  // GL_TRUE for green channel
#define D3DCOLORWRITEENABLE_BLUE    0x00000004  // GL_TRUE for blue channel
#define D3DCOLORWRITEENABLE_ALPHA   0x00000008  // GL_TRUE for alpha channel

// DirectX Flexible Vertex Format (FVF) flags (legacy, unused in modern OpenGL ES)
#define D3DFVF_XYZ              0x002   // Position (x, y, z)
#define D3DFVF_XYZRHW           0x004   // Transformed position (x, y, z, rhw)
#define D3DFVF_NORMAL           0x010   // Normal vector
#define D3DFVF_DIFFUSE          0x040   // Diffuse color
#define D3DFVF_SPECULAR         0x080   // Specular color
#define D3DFVF_TEX1             0x100   // 1 texture coordinate set
#define D3DFVF_TEX2             0x200   // 2 texture coordinate sets
#define D3DFVF_TEX3             0x300   // 3 texture coordinate sets
#define D3DFVF_TEX4             0x400   // 4 texture coordinate sets

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

// DirectX texture arguments (legacy fixed-function)
#define D3DTA_DIFFUSE           0x00000000  // Select diffuse color
#define D3DTA_CURRENT           0x00000001  // Select result of previous stage
#define D3DTA_TEXTURE           0x00000002  // Select texture color
#define D3DTA_TFACTOR           0x00000003  // Select TFACTOR (texture factor constant)
#define D3DTA_SPECULAR          0x00000004  // Select specular color
#define D3DTA_TEMP              0x00000005  // Select temporary register
#define D3DTA_CONSTANT          0x00000006  // Select per-stage constant
#define D3DTA_COMPLEMENT        0x00000010  // Take 1.0 - x
#define D3DTA_ALPHAREPLICATE    0x00000020  // Replicate alpha to color components

// DirectX render states (legacy fixed-function pipeline state)
#define D3DRS_TEXTUREFACTOR     0           // Texture factor constant (RGBA)

// Blender class IDs (shader material types)
#define B_DEFAULT               0
#define B_PARTICLE              1
#define B_SHADOW_TEX            2
#define B_LIGHT                 3

// Render state object (DirectX ID3D11SamplerState equivalent)
// In OpenGL we don't have state objects, just direct state setting
using ID3DState = void*;        // Placeholder - OpenGL doesn't use state objects

// Vertex buffer/index buffer handles (OpenGL uses GLuint for buffer objects)
using VertexBufferHandle = GLuint;
using IndexBufferHandle = GLuint;

// Rendering types from xrEngine
#include "xrEngine/Render.h"

// Property system for shader blenders (xrP_TOKEN, xrP_Integer, xrP_BOOL)
#include "xrEngine/Properties.h"

// Shader resource pointers (ref_shader, ref_geom, ref_constant)
// Include actual definitions from xrRender, not void* placeholders
#include "Layers/xrRender/Shader.h"
#include "Layers/xrRender/Blender.h"           // IBlender base class
#include "Layers/xrRender/Blender_Recorder.h"  // CBlender_Compile definition
#include "Layers/xrRender/R_Backend.h"         // CBackend definition

// Global renderer objects (forward declarations for OpenGL ES)
// These are defined in xrRenderGL backend but referenced in shared xrRender code
namespace xray::render::gl {
    class CRenderTarget;
    class CBackend;
}

// R1 renderer flags (legacy DirectX 9 renderer settings, unused in OpenGL ES)
extern Flags32 ps_r1_flags;
#define R1FLAG_FFP_LIGHTMAPS    (1 << 0)
#define R1FLAG_DLIGHTS          (1 << 1)

// Global renderer implementation object (defined in xrRenderGL, stubbed here)
extern xray::render::RENDER_NAMESPACE::CBackend* RImplementation;

// Global hardware object (CHW - hardware abstraction, defined in xrRenderGL)
// CHW class with immediate context ID constant
namespace xray::render::RENDER_NAMESPACE {
    class CHW {
    public:
        static constexpr u32 IMM_CTX_ID = 0;  // Immediate context ID constant
    };
}

// Global device object (defined in xrEngine, available via GEnv)
#define Device (*GEnv.Render->GetDevice())
