#pragma once

// platform_android.h
// Replaces Windows, Xbox and Sony platform types for Android/Clang builds.
// Include this before any 4J or game headers on Android.

#include <signal.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <string>
#include <wchar.h>

// ─── Prevent Windows headers being pulled in ─────────────────────────────────
#define _WINDOWS_
#define _INC_STDLIB

// ─── SAL annotations — all no-ops on Android ─────────────────────────────────
#define _In_
#define _In_z_
#define _In_opt_
#define _In_opt_z_
#define _Out_
#define _Out_opt_
#define _Outptr_
#define _Inout_
#define _Inout_opt_
#define _In_reads_(x)
#define _In_reads_opt_(x)
#define _Out_writes_(x)
#define _Out_writes_opt_(x)
#define _In_range_(x,y)
#define _Field_size_(x)
#define _Field_size_opt_(x)
#define _Printf_format_string_
#define _Ret_maybenull_
#define _Ret_notnull_
#define _Check_return_
#define _Success_(x)
#define _Must_inspect_result_

// ─── Replace non android things ───────────────────────────────────────────────
#define __debugbreak() raise(SIGTRAP)
#define ZeroMemory(dest, size) memset((dest), 0, (size))

// ─── Basic scalar types ───────────────────────────────────────────────────────
typedef uint8_t   BYTE;
typedef uint8_t   UCHAR;
typedef int8_t    CHAR;
typedef uint16_t  WORD;
typedef int16_t   SHORT;
typedef uint32_t  DWORD;
typedef int32_t   LONG;
typedef uint32_t  ULONG;
typedef int64_t   LONGLONG;
typedef uint64_t  ULONGLONG;
typedef uint64_t  QWORD;
typedef int32_t   BOOL;
typedef int32_t   INT;
typedef uint32_t  UINT;
typedef float     FLOAT;
typedef void      VOID;
typedef size_t    SIZE_T;
typedef uintptr_t ULONG_PTR;
typedef intptr_t  LONG_PTR;
typedef uintptr_t UINT_PTR;
typedef void*     LPVOID;
typedef void*     PVOID;
typedef BYTE*     PBYTE;
typedef long      HRESULT;
typedef DWORD*    LPDWORD;
typedef LONG*     PLONG;

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG  HighPart;
    };
    LONGLONG QuadPart;
} LARGE_INTEGER;

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL nullptr
#endif

// ─── Wide string types ────────────────────────────────────────────────────────
typedef wchar_t        WCHAR;
typedef const wchar_t* LPCWSTR;
typedef wchar_t*       LPWSTR;
typedef const char*    LPCSTR;
typedef char*          LPSTR;
typedef WCHAR*         BSTR;

// ─── HRESULT helpers ──────────────────────────────────────────────────────────
#define S_OK          ((HRESULT)0x00000000L)
#define S_FALSE       ((HRESULT)0x00000001L)
#define E_FAIL        ((HRESULT)0x80004005L)
#define E_NOTIMPL     ((HRESULT)0x80004001L)
#define E_OUTOFMEMORY ((HRESULT)0x8007000EL)
#define E_INVALIDARG  ((HRESULT)0x80070057L)
#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)
#define FAILED(hr)    ((HRESULT)(hr) < 0)

// ─── FILETIME ─────────────────────────────────────────────────────────────────
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;

// ─── SYSTEMTIME ───────────────────────────────────────────────────────────────
typedef struct _SYSTEMTIME {
    WORD wYear, wMonth, wDayOfWeek, wDay;
    WORD wHour, wMinute, wSecond, wMilliseconds;
} SYSTEMTIME;

// ─── Critical section — backed by pthread mutex ───────────────────────────────
typedef struct _CRITICAL_SECTION {
    pthread_mutex_t mutex;
} CRITICAL_SECTION;

inline void InitializeCriticalSectionAndSpinCount(CRITICAL_SECTION* cs, DWORD) {
    pthread_mutex_init(&cs->mutex, nullptr);
}
inline void InitializeCriticalSection(CRITICAL_SECTION* cs) {
    pthread_mutex_init(&cs->mutex, nullptr);
}
inline void DeleteCriticalSection(CRITICAL_SECTION* cs) {
    pthread_mutex_destroy(&cs->mutex);
}
inline void EnterCriticalSection(CRITICAL_SECTION* cs) {
    pthread_mutex_lock(&cs->mutex);
}
inline void LeaveCriticalSection(CRITICAL_SECTION* cs) {
    pthread_mutex_unlock(&cs->mutex);
}
inline bool TryEnterCriticalSection(CRITICAL_SECTION* cs) {
    return pthread_mutex_trylock(&cs->mutex) == 0;
}

// ─── Fake D3D11 types (appear in 4J_Render.h interface) ──────────────────────
struct ID3D11Device             {};
struct IDXGISwapChain           {};
struct ID3D11Buffer             {};
struct ID3D11ShaderResourceView {};

typedef struct tagRECT {
    LONG left, top, right, bottom;
} D3D11_RECT, RECT;

// ─── D3D11 blend/compare constants mapped to OpenGL ES equivalents ────────────
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
#define D3D11_COMPARISON_GREATER        GL_GREATER
#define D3D11_COMPARISON_EQUAL          GL_EQUAL
#define D3D11_COMPARISON_LESS_EQUAL     GL_LEQUAL
#define D3D11_COMPARISON_GREATER_EQUAL  GL_GEQUAL
#define D3D11_COMPARISON_ALWAYS         GL_ALWAYS

// ─── Fake Sony GXM types (in PS Vita 4J_Render.h) ────────────────────────────
struct SceGxmTexture             {};
struct SceGxmContext             {};
struct SceGxmColorSurface        {};
struct SceGxmDepthStencilSurface {};
struct SceGxmShaderPatcher       {};
struct SceGxmNotification        { volatile uint32_t* address; uint32_t value; };
typedef uint32_t SceGxmStencilFunc;

// ─── Sony NP / touch stubs ────────────────────────────────────────────────────
#define SCE_NP_ONLINEID_MAX_LENGTH  16
#define SCE_NET_ETHER_ADDR_LEN       6

struct SceNpOnlineId { char data[SCE_NP_ONLINEID_MAX_LENGTH]; char term; char dummy[3]; };
struct SceNpId       { SceNpOnlineId handle; };
struct SceNpCommunicationConfig { const void* productId; const void* passPhrase; };
typedef int SceMsgDialogSystemMessageType;

#define SCE_TOUCH_MAX_REPORT 6
struct SceTouchReport { uint16_t id, x, y; };
struct SceTouchData {
    uint64_t       timeStamp;
    uint32_t       status;
    uint8_t        reportNum;
    SceTouchReport report[SCE_TOUCH_MAX_REPORT];
};

// ─── PlayerUID / SessionID ────────────────────────────────────────────────────
typedef ULONGLONG  PlayerUID;
typedef ULONGLONG  SessionID;
typedef PlayerUID  GameSessionUID;
typedef PlayerUID* PPlayerUID;
class INVITE_INFO {};

// ─── XUI handle stubs ────────────────────────────────────────────────────────
typedef struct _XUIOBJ*   HXUIOBJ;
typedef struct _XUICLASS* HXUICLASS;
typedef struct _XUIBRUSH* HXUIBRUSH;
typedef struct _XUIXDC*   HXUIDC;

// ─── CXuiStringTable stub ────────────────────────────────────────────────────
class CXuiStringTable {
public:
    LPCWSTR Lookup(LPCWSTR)  { return L""; }
    LPCWSTR Lookup(UINT)     { return L""; }
    void    Clear()          {}
    HRESULT Load(LPCWSTR)    { return S_OK; }
};

// ─── D3DXVECTOR3 ─────────────────────────────────────────────────────────────
class D3DXVECTOR3 {
public:
    float x, y, z, pad;
    D3DXVECTOR3() : x(0), y(0), z(0), pad(0) {}
    D3DXVECTOR3(float x, float y, float z) : x(x), y(y), z(z), pad(0) {}
    D3DXVECTOR3& operator+=(const D3DXVECTOR3& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
};

// ─── XMem compression stubs ──────────────────────────────────────────────────
typedef void* XMEMDECOMPRESSION_CONTEXT;
typedef void* XMEMCOMPRESSION_CONTEXT;
typedef enum { XMEMCODEC_DEFAULT=0, XMEMCODEC_LZX=1 } XMEMCODEC_TYPE;
typedef struct { DWORD Flags, WindowSize, CompressionPartitionSize; } XMEMCODEC_PARAMETERS_LZX;

inline HRESULT XMemDecompress(XMEMDECOMPRESSION_CONTEXT, VOID*, SIZE_T*, const VOID*, SIZE_T) { return E_NOTIMPL; }
inline HRESULT XMemCompress(XMEMCOMPRESSION_CONTEXT, VOID*, SIZE_T*, const VOID*, SIZE_T)     { return E_NOTIMPL; }
inline HRESULT XMemCreateCompressionContext(XMEMCODEC_TYPE, const VOID*, DWORD, XMEMCOMPRESSION_CONTEXT*)   { return E_NOTIMPL; }
inline HRESULT XMemCreateDecompressionContext(XMEMCODEC_TYPE, const VOID*, DWORD, XMEMDECOMPRESSION_CONTEXT*) { return E_NOTIMPL; }
inline void XMemDestroyCompressionContext(XMEMCOMPRESSION_CONTEXT)   {}
inline void XMemDestroyDecompressionContext(XMEMDECOMPRESSION_CONTEXT) {}

// ─── Language / locale stubs ──────────────────────────────────────────────────
inline DWORD XGetLanguage()           { return 0x01; } // English
inline DWORD XGetLocale()             { return 36;   } // United States
inline DWORD XEnableGuestSignin(BOOL) { return 0;    }

// ─── XUserGetSigninInfo stub ──────────────────────────────────────────────────
typedef struct { PlayerUID xuid; DWORD dwGuestNumber; } XUSER_SIGNIN_INFO, *PXUSER_SIGNIN_INFO;
#define XUSER_GET_SIGNIN_INFO_ONLINE_XUID_ONLY  0x00000001
#define XUSER_GET_SIGNIN_INFO_OFFLINE_XUID_ONLY 0x00000002
inline DWORD XUserGetSigninInfo(DWORD, DWORD, PXUSER_SIGNIN_INFO) { return 0; }

// ─── Memory / util stubs ─────────────────────────────────────────────────────
inline void  XMemCpy(void* a, const void* b, size_t s)            { memcpy(a, b, s); }
inline void  XMemSet(void* a, int t, size_t s)                    { memset(a, t, s); }
inline void  XMemSet128(void* a, int t, size_t s)                 { memset(a, t, s); }
inline void* XPhysicalAlloc(SIZE_T a, ULONG_PTR, ULONG_PTR, DWORD) { return malloc(a); }
inline void  XPhysicalFree(void* a)                               { free(a); }
inline bool  IsEqualXUID(PlayerUID a, PlayerUID b)                { return a == b; }

// ─── Misc constants ───────────────────────────────────────────────────────────
#define MULTITHREAD_ENABLE
//typedef unsigned char byte;

const int MINECRAFT_NET_MAX_PLAYERS            = 8;
const int XN_SYS_SIGNINCHANGED                 = 0;
const int XN_SYS_INPUTDEVICESCHANGED           = 1;
const int XN_LIVE_CONTENT_INSTALLED            = 2;
const int XN_SYS_STORAGEDEVICESCHANGED         = 3;
const int QNET_SENDDATA_RELIABLE               = 0;
const int QNET_SENDDATA_SEQUENTIAL             = 0;
const int QNET_GETSENDQUEUESIZE_SECONDARY_TYPE = 0;
const int QNET_GETSENDQUEUESIZE_MESSAGES       = 0;
const int QNET_GETSENDQUEUESIZE_BYTES          = 0;
const int QNET_E_SESSION_FULL                  = 0;
const int QNET_USER_MASK_USER0                 = 1;
const int QNET_USER_MASK_USER1                 = 2;
const int QNET_USER_MASK_USER2                 = 4;
const int QNET_USER_MASK_USER3                 = 8;
const int D3DBLEND_CONSTANTALPHA               = 0;
const int D3DBLEND_INVCONSTANTALPHA            = 0;
const int D3DPT_QUADLIST                       = 0;
const int XZP_ICON_SHANK_01                    = 1;
const int XZP_ICON_SHANK_02                    = 2;
const int XZP_ICON_SHANK_03                    = 3;

// ─── OutputDebugString → Android logcat ──────────────────────────────────────
#include <android/log.h>
#define OutputDebugString(s)  __android_log_print(ANDROID_LOG_DEBUG, "MC", "%ls", (s))
#define OutputDebugStringA(s) __android_log_print(ANDROID_LOG_DEBUG, "MC", "%s",  (s))
#define OutputDebugStringW(s) __android_log_print(ANDROID_LOG_DEBUG, "MC", "%ls", (s))

// ─── STL using ────────────────────────────────────────────────────────────────
using std::wstring;
using std::string;