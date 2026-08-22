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
