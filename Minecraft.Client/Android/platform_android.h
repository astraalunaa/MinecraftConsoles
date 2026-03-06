#pragma once

// platform_android.h
// Replaces Windows and Sony platform types for Android builds.
// Include this before any 4J headers on Android.

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string>
#include <wchar.h>

// ─── Basic Windows scalar types ───────────────────────────────────────────────
typedef uint8_t   BYTE;
typedef uint16_t  WORD;
typedef uint32_t  DWORD;
typedef uint64_t  QWORD;
typedef int32_t   BOOL;
typedef float     FLOAT;
typedef unsigned int UINT;
typedef void*     LPVOID;
typedef long      HRESULT;
typedef uint8_t*  PBYTE;

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// ─── Wide string types ────────────────────────────────────────────────────────
typedef wchar_t         WCHAR;
typedef const wchar_t*  LPCWSTR;
typedef wchar_t*        LPWSTR;

// ─── HRESULT helpers ──────────────────────────────────────────────────────────
#define S_OK       ((HRESULT)0x00000000L)
#define S_FALSE    ((HRESULT)0x00000001L)
#define E_FAIL     ((HRESULT)0x80004005L)
#define E_NOTIMPL  ((HRESULT)0x80004001L)
#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)
#define FAILED(hr)    ((HRESULT)(hr) < 0)

// ─── Fake D3D11 types (used in 4J_Render.h interface) ────────────────────────
// These are opaque stand-ins so the header compiles.
// The Android C4JRender implementation never uses them.
struct ID3D11Device           {};
struct IDXGISwapChain         {};
struct ID3D11Buffer           {};
struct ID3D11ShaderResourceView {};

// D3D11 rect (used in Clear())
typedef struct tagRECT {
    int left, top, right, bottom;
} D3D11_RECT;

// D3D11 blend constants — mapped to OpenGL ES equivalents
// Values match GLES glBlendFunc tokens so they can be passed through directly.
#include <GLES3/gl3.h>
#define D3D11_BLEND_SRC_ALPHA           GL_SRC_ALPHA
#define D3D11_BLEND_INV_SRC_ALPHA       GL_ONE_MINUS_SRC_ALPHA
#define D3D11_BLEND_ONE                 GL_ONE
#define D3D11_BLEND_ZERO                GL_ZERO
#define D3D11_BLEND_DEST_ALPHA          GL_DST_ALPHA
#define D3D11_BLEND_SRC_COLOR           GL_SRC_COLOR
#define D3D11_BLEND_DEST_COLOR          GL_DST_COLOR
#define D3D11_BLEND_INV_DEST_COLOR      GL_ONE_MINUS_DST_COLOR
#define D3D11_BLEND_INV_SRC_COLOR       GL_ONE_MINUS_SRC_COLOR
#define D3D11_BLEND_BLEND_FACTOR        GL_CONSTANT_ALPHA
#define D3D11_BLEND_INV_BLEND_FACTOR    GL_ONE_MINUS_CONSTANT_ALPHA

// D3D11 comparison constants — mapped to GLES depth/stencil func tokens
#define D3D11_COMPARISON_GREATER        GL_GREATER
#define D3D11_COMPARISON_EQUAL          GL_EQUAL
#define D3D11_COMPARISON_LESS_EQUAL     GL_LEQUAL
#define D3D11_COMPARISON_GREATER_EQUAL  GL_GEQUAL
#define D3D11_COMPARISON_ALWAYS         GL_ALWAYS

// ─── Fake Sony/Vita GXM types (used in PS Vita 4J_Render.h) ──────────────────
// Only needed if compiling the shared header that still has Vita remnants.
struct SceGxmTexture          {};
struct SceGxmContext          {};
struct SceGxmColorSurface     {};
struct SceGxmDepthStencilSurface {};
struct SceGxmShaderPatcher    {};
struct SceGxmNotification     { volatile uint32_t* address; uint32_t value; };
typedef uint32_t SceGxmStencilFunc;

// ─── Fake Sony NP / touch types (used in 4J_Profile.h / 4J_Input.h) ──────────
#define SCE_NP_ONLINEID_MAX_LENGTH  16
#define SCE_NET_ETHER_ADDR_LEN       6
#define XUSER_INDEX_ANY             0xFF

struct SceNpOnlineId  { char data[SCE_NP_ONLINEID_MAX_LENGTH]; char term; char dummy[3]; };
struct SceNpId        { SceNpOnlineId handle; };
struct SceNpCommunicationConfig { const void* productId; const void* passPhrase; };
typedef int SceMsgDialogSystemMessageType;

// SceTouchData stub — MapTouchInput / GetTouchPadData use this
#define SCE_TOUCH_MAX_REPORT 6
struct SceTouchReport {
    uint16_t id;
    uint16_t x;
    uint16_t y;
};
struct SceTouchData {
    uint64_t   timeStamp;
    uint32_t   status;
    uint8_t    reportNum;
    SceTouchReport report[SCE_TOUCH_MAX_REPORT];
};

// ─── Xbox / Windows extras referenced in Profile ─────────────────────────────
typedef void* CXuiStringTable;
typedef PlayerUID* PPlayerUID;  // forward — PlayerUID defined in 4J_Profile.h

// wstring used in Profile header
using std::wstring;