// 4J_Render_Android.cpp
// Stub implementation of C4JRender for Android.
// All methods are no-ops or return safe defaults.
// Replace each method body with real OpenGL ES 3.0 code incrementally.

#include "platform_android.h"
#include "4J_Render.h"
#include <android/log.h>

#define LOG_TAG "4JRender"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)

// Global singleton instance
C4JRender RenderManager;

// ─── Core ─────────────────────────────────────────────────────────────────────

void C4JRender::Initialise()
{
    LOGI("C4JRender::Initialise");
    // TODO: EGL surface and context setup goes here
}

void C4JRender::InitialiseContext()
{
    LOGI("C4JRender::InitialiseContext");
    // TODO: OpenGL ES state initialisation
}

void C4JRender::Tick() {}

void C4JRender::UpdateGamma(unsigned short usGamma) {}

void C4JRender::StartFrame()
{
    // TODO: glClear etc.
}

void C4JRender::Present()
{
    // TODO: eglSwapBuffers
}

void C4JRender::Clear(int flags, D3D11_RECT *pRect)
{
    // TODO: glClear(flags mapped to GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
}

void C4JRender::SetClearColour(const float colourRGBA[4])
{
    // TODO: glClearColor
}

bool C4JRender::IsWidescreen()  { return true; }
bool C4JRender::IsHiDef()       { return true; }

void C4JRender::CaptureThumbnail(ImageFileBuffer *pngOut) {}
void C4JRender::CaptureScreen(ImageFileBuffer *jpgOut, XSOCIAL_PREVIEWIMAGE *previewOut) {}

void C4JRender::BeginConditionalSurvey(int identifier) {}
void C4JRender::EndConditionalSurvey() {}
void C4JRender::BeginConditionalRendering(int identifier) {}
void C4JRender::EndConditionalRendering() {}

// ─── Matrix stack ─────────────────────────────────────────────────────────────
// TODO: implement a software matrix stack (glm or hand-rolled 4x4)

void C4JRender::MatrixMode(int type) {}
void C4JRender::MatrixSetIdentity() {}
void C4JRender::MatrixTranslate(float x, float y, float z) {}
void C4JRender::MatrixRotate(float angle, float x, float y, float z) {}
void C4JRender::MatrixScale(float x, float y, float z) {}
void C4JRender::MatrixPerspective(float fovy, float aspect, float zNear, float zFar) {}
void C4JRender::MatrixOrthogonal(float l, float r, float b, float t, float zn, float zf) {}
void C4JRender::MatrixPop() {}
void C4JRender::MatrixPush() {}
void C4JRender::MatrixMult(float *mat) {}
const float *C4JRender::MatrixGet(int type) { static float identity[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1}; return identity; }
void C4JRender::Set_matrixDirty() {}

// ─── Vertex drawing ───────────────────────────────────────────────────────────

void C4JRender::DrawVertices(ePrimitiveType pt, int count, void *dataIn, eVertexType vType, ePixelShaderType psType)
{
    // TODO: upload to a temporary VBO and glDrawArrays
}

void C4JRender::DrawVertexBuffer(ePrimitiveType pt, int count, ID3D11Buffer *buffer, eVertexType vType, ePixelShaderType psType)
{
    // TODO: buffer is a stub on Android — use internal VBO handle instead
}

// ─── Command buffers ──────────────────────────────────────────────────────────

void C4JRender::CBuffLockStaticCreations() {}
int  C4JRender::CBuffCreate(int count)     { return 0; }
void C4JRender::CBuffDelete(int first, int count) {}
void C4JRender::CBuffStart(int index)      {}
void C4JRender::CBuffClear(int index)      {}
int  C4JRender::CBuffSize(int index)       { return 0; }
void C4JRender::CBuffEnd()                 {}
bool C4JRender::CBuffCall(int index, bool full) { return false; }
void C4JRender::CBuffTick()                {}
void C4JRender::CBuffDeferredModeStart()   {}
void C4JRender::CBuffDeferredModeEnd()     {}

// ─── Textures ─────────────────────────────────────────────────────────────────

int  C4JRender::TextureCreate()            { return 0; /* TODO: glGenTextures */ }
void C4JRender::TextureFree(int idx)       { /* TODO: glDeleteTextures */ }
void C4JRender::TextureBind(int idx)       { /* TODO: glBindTexture */ }
void C4JRender::TextureBind(int layer, int idx) {}
void C4JRender::TextureBindVertex(int idx) {}
void C4JRender::TextureSetTextureLevels(int levels) {}
int  C4JRender::TextureGetTextureLevels()  { return 1; }

unsigned char* C4JRender::TextureData(int width, int height, void *data, int level, eTextureFormat format)
{
    // TODO: glTexImage2D
    return nullptr;
}

void C4JRender::TextureDataUpdate(int xoffset, int yoffset, int width, int height, void *data, int level)
{
    // TODO: glTexSubImage2D
}

void C4JRender::TextureSetParam(int param, int value) { /* TODO: glTexParameteri */ }
void C4JRender::TextureDynamicUpdateStart() {}
void C4JRender::TextureDynamicUpdateEnd()   {}

HRESULT C4JRender::LoadTextureData(const char *szFilename, D3DXIMAGE_INFO *pSrcInfo, int **ppDataOut)
{
    // TODO: load PNG/JPG via Android asset manager
    return E_NOTIMPL;
}

HRESULT C4JRender::LoadTextureData(BYTE *pbData, DWORD dwBytes, D3DXIMAGE_INFO *pSrcInfo, int **ppDataOut)
{
    return E_NOTIMPL;
}

HRESULT C4JRender::SaveTextureData(const char *szFilename, D3DXIMAGE_INFO *pSrcInfo, int *ppDataOut)
{
    return E_NOTIMPL;
}

void C4JRender::TextureGetStats() {}

SceGxmTexture* C4JRender::TextureGetTexture(int idx)
{
    // Vita-specific — not called on Android, safe to return null
    return nullptr;
}

// ─── State control ────────────────────────────────────────────────────────────

void C4JRender::StateSetColour(float r, float g, float b, float a) {}
void C4JRender::StateSetDepthMask(bool enable)          { /* TODO: glDepthMask */ }
void C4JRender::StateSetBlendEnable(bool enable)        { /* TODO: glEnable/glDisable GL_BLEND */ }
void C4JRender::StateSetBlendFunc(int src, int dst)     { /* TODO: glBlendFunc */ }
void C4JRender::StateSetBlendFactor(unsigned int colour){}
void C4JRender::StateSetAlphaFunc(int func, float param){}  // not in GLES3 — emulate in shader
void C4JRender::StateSetDepthFunc(int func)             { /* TODO: glDepthFunc */ }
void C4JRender::StateSetFaceCull(bool enable)           { /* TODO: glEnable GL_CULL_FACE */ }
void C4JRender::StateSetFaceCullCW(bool enable)         { /* TODO: glFrontFace */ }
void C4JRender::StateSetLineWidth(float width)          { /* TODO: glLineWidth */ }
void C4JRender::StateSetWriteEnable(bool r, bool g, bool b, bool a) { /* TODO: glColorMask */ }
void C4JRender::StateSetDepthTestEnable(bool enable)    { /* TODO: glEnable GL_DEPTH_TEST */ }
void C4JRender::StateSetAlphaTestEnable(bool enable)    {}  // emulate in shader
void C4JRender::StateSetDepthSlopeAndBias(float slope, float bias) { /* TODO: glPolygonOffset */ }
void C4JRender::StateSetFogEnable(bool enable)          {}  // emulate in shader
void C4JRender::StateSetFogMode(int mode)               {}
void C4JRender::StateSetFogNearDistance(float dist)     {}
void C4JRender::StateSetFogFarDistance(float dist)      {}
void C4JRender::StateSetFogDensity(float density)       {}
void C4JRender::StateSetFogColour(float r, float g, float b) {}
void C4JRender::StateSetLightingEnable(bool enable)     {}
void C4JRender::StateSetVertexTextureUV(float u, float v) {}
void C4JRender::StateSetLightColour(int light, float r, float g, float b) {}
void C4JRender::StateSetLightAmbientColour(float r, float g, float b) {}
void C4JRender::StateSetLightDirection(int light, float x, float y, float z) {}
void C4JRender::StateSetLightEnable(int light, bool enable) {}
void C4JRender::StateSetViewport(eViewportType viewportType) { /* TODO: glViewport */ }
void C4JRender::StateSetEnableViewportClipPlanes(bool enable) {}
void C4JRender::StateSetTexGenCol(int col, float x, float y, float z, float w, bool eyeSpace) {}
void C4JRender::StateSetStencil(SceGxmStencilFunc func, uint8_t func_mask, uint8_t write_mask) { /* TODO: glStencilFunc */ }
void C4JRender::StateSetForceLOD(int LOD) {}

// ─── Events / PLM ─────────────────────────────────────────────────────────────

void C4JRender::BeginEvent(LPCWSTR eventName) {}
void C4JRender::EndEvent()                    {}

// ─── Vita/PS4 GXM memory — stubs, never called on Android ────────────────────

void* C4JRender::allocGPURWMem(const size_t size, const size_t alignment)  { return nullptr; }
void  C4JRender::freeGPURWMem(void* mem)                                    {}
void* C4JRender::allocGPUROMem(const size_t size, const size_t alignment)  { return nullptr; }
void  C4JRender::freeGPUROMem(void* mem)                                    {}
void* C4JRender::allocGPUCDMem(const size_t size, const size_t alignment)  { return nullptr; }
void  C4JRender::freeGPUCDMem(void* mem)                                    {}
int   C4JRender::getDisplayBufferCount()                                    { return 2; }
volatile uint32_t* C4JRender::getNotificationRegion()                       { return nullptr; }
SceGxmContext*         C4JRender::getGXMContext()                           { return nullptr; }
SceGxmColorSurface*    C4JRender::getCurrentDisplaySurface()                { return nullptr; }
SceGxmDepthStencilSurface* C4JRender::getCurrentDepthSurface()              { return nullptr; }
SceGxmShaderPatcher*   C4JRender::getGXMShaderPatcher()                     { return nullptr; }
void  C4JRender::setFragmentNotification(SceGxmNotification n)              {}
bool  C4JRender::GetIsInScene()                                             { return false; }
void  C4JRender::SetCameraPosition(float x, float y, float z)              {}