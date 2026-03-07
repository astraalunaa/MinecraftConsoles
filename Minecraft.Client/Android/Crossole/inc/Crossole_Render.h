#pragma once

// Crossole_Render.h
// Android render abstraction layer for MinecraftConsoles.
// Based on 4J Studios' render interface, reimplemented for Android OpenGL ES 3.0.
// Sources: Windows64, PSVita 4J_Render headers.
//
// IMPORTANT: Do NOT include GLES3/gl3.h here.
// The GL_* constants below are 4J's own internal enum values, NOT real OpenGL.
// Real GLES3 constants are only used inside Crossole_Render.cpp.

#include "platform_android.h"
#include <stdint.h>

// ─── Image buffer — used for thumbnails and screenshots ──────────────────────
class ImageFileBuffer
{
public:
	enum EImageType { e_typePNG, e_typeJPG };

	EImageType  m_type;
	void*       m_pBuffer;
	int         m_bufferSize;

	int     GetType()           { return m_type; }
	void*   GetBufferPointer()  { return m_pBuffer; }
	int     GetBufferSize()     { return m_bufferSize; }
	void    Release()           { free(m_pBuffer); m_pBuffer = NULL; }
	bool    Allocated()         { return m_pBuffer != NULL; }
};

typedef struct { int Width; int Height; } D3DXIMAGE_INFO;

typedef struct _XSOCIAL_PREVIEWIMAGE {
	BYTE*  pBytes;
	DWORD  Pitch;
	DWORD  Width;
	DWORD  Height;
} XSOCIAL_PREVIEWIMAGE, *PXSOCIAL_PREVIEWIMAGE;

// ─── C4JRender ────────────────────────────────────────────────────────────────
class C4JRender
{
public:

	// ── Vertex types — superset of Windows64 + PSVita ────────────────────────
	typedef enum
	{
		VERTEX_TYPE_PF3_TF2_CB4_NB4_XW1,
		VERTEX_TYPE_COMPRESSED,
		VERTEX_TYPE_PF3_TF2_CB4_NB4_XW1_LIT,
		VERTEX_TYPE_PF3_TF2_CB4_NB4_XW1_TEXGEN,
		VERTEX_TYPE_COMPRESSED_FOG_1,       // PSVita addition
		VERTEX_TYPE_COMPRESSED_FOG_2,       // PSVita addition
		VERTEX_TYPE_COUNT
	} eVertexType;

	// ── Pixel shader types — superset of Windows64 + PSVita ──────────────────
	typedef enum
	{
		PIXEL_SHADER_TYPE_STANDARD,
		PIXEL_SHADER_TYPE_STANDARD2,        // PSVita addition
		PIXEL_SHADER_TYPE_STANDARD3,        // PSVita addition
		PIXEL_SHADER_TYPE_STANDARD4,        // PSVita addition
		PIXEL_SHADER_TYPE_PROJECTION,
		PIXEL_SHADER_TYPE_FORCELOD,         // Windows64 addition
		PIXEL_SHADER_COUNT
	} ePixelShaderType;

	// ── Viewport types ────────────────────────────────────────────────────────
	typedef enum
	{
		VIEWPORT_TYPE_FULLSCREEN,
		VIEWPORT_TYPE_SPLIT_TOP,
		VIEWPORT_TYPE_SPLIT_BOTTOM,
		VIEWPORT_TYPE_SPLIT_LEFT,
		VIEWPORT_TYPE_SPLIT_RIGHT,
		VIEWPORT_TYPE_QUADRANT_TOP_LEFT,
		VIEWPORT_TYPE_QUADRANT_TOP_RIGHT,
		VIEWPORT_TYPE_QUADRANT_BOTTOM_LEFT,
		VIEWPORT_TYPE_QUADRANT_BOTTOM_RIGHT,
	} eViewportType;

	// ── Primitive types ───────────────────────────────────────────────────────
	typedef enum
	{
		PRIMITIVE_TYPE_TRIANGLE_LIST,
		PRIMITIVE_TYPE_TRIANGLE_STRIP,
		PRIMITIVE_TYPE_TRIANGLE_FAN,
		PRIMITIVE_TYPE_QUAD_LIST,
		PRIMITIVE_TYPE_LINE_LIST,
		PRIMITIVE_TYPE_LINE_STRIP,
		PRIMITIVE_TYPE_COUNT
	} ePrimitiveType;

	// ── Texture formats ───────────────────────────────────────────────────────
	typedef enum
	{
		TEXTURE_FORMAT_RxGyBzAw,
		MAX_TEXTURE_FORMATS
	} eTextureFormat;

	// ── Init / tick ───────────────────────────────────────────────────────────
	// Android: no D3D device params — EGL context is set up internally
	void    Initialise();
	void    InitialiseContext();
	void    Tick();
	void    UpdateGamma(unsigned short usGamma);

	// ── Frame ─────────────────────────────────────────────────────────────────
	void    StartFrame();
	void    Present();
	void    Clear(int flags, D3D11_RECT* pRect = NULL);
	void    SetClearColour(const float colourRGBA[4]);
	void    DoScreenGrabOnNextPresent();

	// ── Display info ──────────────────────────────────────────────────────────
	bool    IsWidescreen();
	bool    IsHiDef();

	// ── Screenshots / thumbnails ──────────────────────────────────────────────
	void    CaptureThumbnail(ImageFileBuffer* pngOut);
	void    CaptureScreen(ImageFileBuffer* jpgOut, XSOCIAL_PREVIEWIMAGE* previewOut);

	// ── Conditional rendering (survey / occlusion) ────────────────────────────
	void    BeginConditionalSurvey(int identifier);
	void    EndConditionalSurvey();
	void    BeginConditionalRendering(int identifier);
	void    EndConditionalRendering();

	// ── Matrix stack ──────────────────────────────────────────────────────────
	void            MatrixMode(int type);
	void            MatrixSetIdentity();
	void            MatrixTranslate(float x, float y, float z);
	void            MatrixRotate(float angle, float x, float y, float z);
	void            MatrixScale(float x, float y, float z);
	void            MatrixPerspective(float fovy, float aspect, float zNear, float zFar);
	void            MatrixOrthogonal(float left, float right, float bottom, float top, float zNear, float zFar);
	void            MatrixPop();
	void            MatrixPush();
	void            MatrixMult(float* mat);
	const float*    MatrixGet(int type);
	void            Set_matrixDirty();

	// ── Draw calls ────────────────────────────────────────────────────────────
	void    DrawVertices(ePrimitiveType PrimitiveType, int count, void* dataIn,
						 eVertexType vType, C4JRender::ePixelShaderType psType);
	void    DrawVertexBuffer(ePrimitiveType PrimitiveType, int count, ID3D11Buffer* buffer,
							 C4JRender::eVertexType vType, C4JRender::ePixelShaderType psType);

	// ── Command buffers ───────────────────────────────────────────────────────
	void    CBuffLockStaticCreations();
	int     CBuffCreate(int count);
	void    CBuffDelete(int first, int count);
	void    CBuffStart(int index, bool full = false);
	void    CBuffClear(int index);
	int     CBuffSize(int index);
	void    CBuffEnd();
	bool    CBuffCall(int index, bool full = true);
	void    CBuffTick();
	void    CBuffDeferredModeStart();
	void    CBuffDeferredModeEnd();

	// ── Textures ──────────────────────────────────────────────────────────────
	int             TextureCreate();
	void            TextureFree(int idx);
	void            TextureBind(int idx);
	void            TextureBind(int layer, int idx);    // PSVita multi-layer
	void            TextureBindVertex(int idx);
	void            TextureSetTextureLevels(int levels);
	int             TextureGetTextureLevels();
	// Returns pointer to raw texture data (used by PSVita GXM path)
	// On Android returns nullptr — use TextureBind instead
	void*           TextureGetTexture(int idx);
	unsigned char*  TextureData(int width, int height, void* data, int level,
								eTextureFormat format = TEXTURE_FORMAT_RxGyBzAw);
	void            TextureDataUpdate(int xoffset, int yoffset, int width, int height,
									  void* data, int level);
	void            TextureSetParam(int param, int value);
	void            TextureDynamicUpdateStart();
	void            TextureDynamicUpdateEnd();
	HRESULT         LoadTextureData(const char* szFilename, D3DXIMAGE_INFO* pSrcInfo,
									int** ppDataOut);
	HRESULT         LoadTextureData(BYTE* pbData, DWORD dwBytes, D3DXIMAGE_INFO* pSrcInfo,
									int** ppDataOut);
	HRESULT         SaveTextureData(const char* szFilename, D3DXIMAGE_INFO* pSrcInfo,
									int* ppDataOut);
	HRESULT         SaveTextureDataToMemory(void* pOutput, int outputCapacity,
											int* outputLength, int width, int height,
											int* ppDataIn);
	void            TextureGetStats();

	// ── State control ─────────────────────────────────────────────────────────
	void    StateSetColour(float r, float g, float b, float a);
	void    StateSetDepthMask(bool enable);
	void    StateSetBlendEnable(bool enable);
	void    StateSetBlendFunc(int src, int dst);
	void    StateSetBlendFactor(unsigned int colour);
	void    StateSetAlphaFunc(int func, float param);
	void    StateSetDepthFunc(int func);
	void    StateSetFaceCull(bool enable);
	void    StateSetFaceCullCW(bool enable);
	void    StateSetLineWidth(float width);
	void    StateSetWriteEnable(bool red, bool green, bool blue, bool alpha);
	void    StateSetDepthTestEnable(bool enable);
	void    StateSetAlphaTestEnable(bool enable);
	void    StateSetDepthSlopeAndBias(float slope, float bias);
	void    StateSetFogEnable(bool enable);
	void    StateSetFogMode(int mode);
	void    StateSetFogNearDistance(float dist);
	void    StateSetFogFarDistance(float dist);
	void    StateSetFogDensity(float density);
	void    StateSetFogColour(float red, float green, float blue);
	void    StateSetLightingEnable(bool enable);
	void    StateSetVertexTextureUV(float u, float v);
	void    StateSetLightColour(int light, float red, float green, float blue);
	void    StateSetLightAmbientColour(float red, float green, float blue);
	void    StateSetLightDirection(int light, float x, float y, float z);
	void    StateSetLightEnable(int light, bool enable);
	void    StateSetViewport(eViewportType viewportType);
	void    StateSetEnableViewportClipPlanes(bool enable);
	void    StateSetTexGenCol(int col, float x, float y, float z, float w, bool eyeSpace);
	// Android uses plain int for stencil func — no GXM or D3D11 enum dependency
	void    StateSetStencil(int Function, uint8_t stencil_ref,
							uint8_t stencil_func_mask, uint8_t stencil_write_mask);
	void    StateSetForceLOD(int LOD);
	bool    GetIsInScene();
	void    SetCameraPosition(float x, float y, float z);

	// ── Event tracking (debug markers) ───────────────────────────────────────
	void    BeginEvent(LPCWSTR eventName);
	void    EndEvent();

	// ── Lifecycle ─────────────────────────────────────────────────────────────
	void    Suspend();
	bool    Suspended();
	void    Resume();
};

// ─── 4J internal GL-named constants ──────────────────────────────────────────
// These are NOT real OpenGL values — they are 4J's own internal enums
// that happen to use GL_ naming. They map to GLES3 equivalents inside
// Crossole_Render.cpp only.
//
// We use an anonymous namespace to prevent these from conflicting with
// any real GL_ macros if GLES3 headers are ever included in the same TU.
namespace Crossole4J
{
	const int MODELVIEW_MATRIX  = 0;
	const int PROJECTION_MATRIX = 1;
	const int MODELVIEW         = 0;
	const int PROJECTION        = 1;
	const int TEXTURE_MATRIX    = 2;    // renamed from C4J_TEXTURE to avoid GLES3 clash

	// Tex gen
	const int S = 0;
	const int T = 1;
	const int R = 2;
	const int Q = 3;

	const int TEXTURE_GEN_S    = 0;
	const int TEXTURE_GEN_T    = 1;
	const int TEXTURE_GEN_Q    = 2;
	const int TEXTURE_GEN_R    = 3;
	const int TEXTURE_GEN_MODE = 0;
	const int OBJECT_LINEAR    = 0;
	const int EYE_LINEAR       = 1;
	const int OBJECT_PLANE     = 0;
	const int EYE_PLANE        = 1;

	// Enable/disable tokens
	const int TEXTURE_2D  = 1;
	const int BLEND       = 2;
	const int CULL_FACE   = 3;
	const int ALPHA_TEST  = 4;
	const int DEPTH_TEST  = 5;
	const int FOG         = 6;
	const int LIGHTING    = 7;
	const int LIGHT0      = 8;
	const int LIGHT1      = 9;

	// Clear flags
	const int CLEAR_DEPTH_FLAG  = 1;
	const int CLEAR_COLOUR_FLAG = 2;
	const int DEPTH_BUFFER_BIT  = CLEAR_DEPTH_FLAG;
	const int COLOR_BUFFER_BIT  = CLEAR_COLOUR_FLAG;

	// Blend factors — arbitrary internal values, mapped to GLES3 in .cpp
	const int SRC_ALPHA              = 1;
	const int ONE_MINUS_SRC_ALPHA    = 2;
	const int ONE                    = 3;
	const int ZERO                   = 4;
	const int DST_ALPHA              = 5;
	const int SRC_COLOR              = 6;
	const int DST_COLOR              = 7;
	const int ONE_MINUS_DST_COLOR    = 8;
	const int ONE_MINUS_SRC_COLOR    = 9;
	const int CONSTANT_ALPHA         = 10;
	const int ONE_MINUS_CONSTANT_ALPHA = 11;

	// Depth/alpha compare functions
	const int GREATER  = 1;
	const int EQUAL    = 2;
	const int LEQUAL   = 3;
	const int GEQUAL   = 4;
	const int ALWAYS   = 5;

	// Texture params
	const int TEXTURE_MIN_FILTER     = 1;
	const int TEXTURE_MAG_FILTER     = 2;
	const int TEXTURE_WRAP_S         = 3;
	const int TEXTURE_WRAP_T         = 4;
	const int NEAREST                = 0;
	const int LINEAR                 = 1;
	const int EXP                    = 2;
	const int NEAREST_MIPMAP_LINEAR  = 0;
	const int CLAMP                  = 0;
	const int REPEAT                 = 1;

	// Fog params
	const int FOG_START   = 1;
	const int FOG_END     = 2;
	const int FOG_MODE    = 3;
	const int FOG_DENSITY = 4;
	const int FOG_COLOR   = 5;

	// Light params
	const int POSITION          = 1;
	const int AMBIENT           = 2;
	const int DIFFUSE           = 3;
	const int SPECULAR          = 4;
	const int LIGHT_MODEL_AMBIENT = 1;
}

// ─── Global GL_ aliases — point to Crossole4J namespace ──────────────────────
// Game code uses C4JBLEND etc. directly. These bring them into global scope
// without conflicting with GLES3 macros since the GLES3 headers are never
// included in this file.
const int GL_MODELVIEW_MATRIX       = Crossole4J::MODELVIEW_MATRIX;
const int GL_PROJECTION_MATRIX      = Crossole4J::PROJECTION_MATRIX;
const int GL_MODELVIEW              = Crossole4J::MODELVIEW;
const int GL_PROJECTION             = Crossole4J::PROJECTION;
const int C4J_TEXTURE_MATRIX         = Crossole4J::TEXTURE_MATRIX;

const int GL_S                      = Crossole4J::S;
const int GL_T                      = Crossole4J::T;
const int GL_R                      = Crossole4J::R;
const int GL_Q                      = Crossole4J::Q;

const int C4J_TEXTURE_GEN_S          = Crossole4J::TEXTURE_GEN_S;
const int C4J_TEXTURE_GEN_T          = Crossole4J::TEXTURE_GEN_T;
const int C4J_TEXTURE_GEN_Q          = Crossole4J::TEXTURE_GEN_Q;
const int C4J_TEXTURE_GEN_R          = Crossole4J::TEXTURE_GEN_R;
const int C4J_TEXTURE_GEN_MODE       = Crossole4J::TEXTURE_GEN_MODE;
const int GL_OBJECT_LINEAR          = Crossole4J::OBJECT_LINEAR;
const int GL_EYE_LINEAR             = Crossole4J::EYE_LINEAR;
const int GL_OBJECT_PLANE           = Crossole4J::OBJECT_PLANE;
const int GL_EYE_PLANE              = Crossole4J::EYE_PLANE;

const int C4JTEXTURE_2D             = Crossole4J::TEXTURE_2D;
const int C4JBLEND                  = Crossole4J::BLEND;
const int C4JCULL_FACE              = Crossole4J::CULL_FACE;
const int C4JALPHA_TEST             = Crossole4J::ALPHA_TEST;
const int C4JDEPTH_TEST             = Crossole4J::DEPTH_TEST;
const int GL_FOG                    = Crossole4J::FOG;
const int GL_LIGHTING               = Crossole4J::LIGHTING;
const int GL_LIGHT0                 = Crossole4J::LIGHT0;
const int GL_LIGHT1                 = Crossole4J::LIGHT1;

const int CLEAR_DEPTH_FLAG          = Crossole4J::CLEAR_DEPTH_FLAG;
const int CLEAR_COLOUR_FLAG         = Crossole4J::CLEAR_COLOUR_FLAG;
const int C4J_DEPTH_BUFFER_BIT       = Crossole4J::DEPTH_BUFFER_BIT;
const int C4J_COLOR_BUFFER_BIT       = Crossole4J::COLOR_BUFFER_BIT;

const int C4J_SRC_ALPHA              = Crossole4J::SRC_ALPHA;
const int C4J_ONE_MINUS_SRC_ALPHA    = Crossole4J::ONE_MINUS_SRC_ALPHA;
const int C4J_ONE                    = Crossole4J::ONE;
const int C4J_ZERO                   = Crossole4J::ZERO;
const int C4J_DST_ALPHA              = Crossole4J::DST_ALPHA;
const int C4J_SRC_COLOR              = Crossole4J::SRC_COLOR;
const int C4J_DST_COLOR              = Crossole4J::DST_COLOR;
const int C4J_ONE_MINUS_DST_COLOR    = Crossole4J::ONE_MINUS_DST_COLOR;
const int C4J_ONE_MINUS_SRC_COLOR    = Crossole4J::ONE_MINUS_SRC_COLOR;
const int C4J_CONSTANT_ALPHA         = Crossole4J::CONSTANT_ALPHA;
const int C4J_ONE_MINUS_CONSTANT_ALPHA = Crossole4J::ONE_MINUS_CONSTANT_ALPHA;

const int C4J_GREATER                = Crossole4J::GREATER;
const int C4J_EQUAL                  = Crossole4J::EQUAL;
const int C4J_LEQUAL                 = Crossole4J::LEQUAL;
const int C4J_GEQUAL                 = Crossole4J::GEQUAL;
const int C4J_ALWAYS                 = Crossole4J::ALWAYS;

const int C4J_TEXTURE_MIN_FILTER     = Crossole4J::TEXTURE_MIN_FILTER;
const int C4J_TEXTURE_MAG_FILTER     = Crossole4J::TEXTURE_MAG_FILTER;
const int C4J_TEXTURE_WRAP_S         = Crossole4J::TEXTURE_WRAP_S;
const int C4J_TEXTURE_WRAP_T         = Crossole4J::TEXTURE_WRAP_T;
const int C4J_NEAREST                = Crossole4J::NEAREST;
const int C4J_LINEAR                 = Crossole4J::LINEAR;
const int GL_EXP                    = Crossole4J::EXP;
const int C4J_NEAREST_MIPMAP_LINEAR  = Crossole4J::NEAREST_MIPMAP_LINEAR;
const int GL_CLAMP                  = Crossole4J::CLAMP;
const int C4J_REPEAT                 = Crossole4J::REPEAT;

const int GL_FOG_START              = Crossole4J::FOG_START;
const int GL_FOG_END                = Crossole4J::FOG_END;
const int GL_FOG_MODE               = Crossole4J::FOG_MODE;
const int GL_FOG_DENSITY            = Crossole4J::FOG_DENSITY;
const int GL_FOG_COLOR              = Crossole4J::FOG_COLOR;

const int GL_POSITION               = Crossole4J::POSITION;
const int GL_AMBIENT                = Crossole4J::AMBIENT;
const int GL_DIFFUSE                = Crossole4J::DIFFUSE;
const int GL_SPECULAR               = Crossole4J::SPECULAR;
const int GL_LIGHT_MODEL_AMBIENT    = Crossole4J::LIGHT_MODEL_AMBIENT;

const int C4J_LINES          = C4JRender::PRIMITIVE_TYPE_LINE_LIST;
const int C4J_LINE_STRIP     = C4JRender::PRIMITIVE_TYPE_LINE_STRIP;
const int GL_QUADS          = C4JRender::PRIMITIVE_TYPE_QUAD_LIST;
const int C4J_TRIANGLE_FAN   = C4JRender::PRIMITIVE_TYPE_TRIANGLE_FAN;
const int C4J_TRIANGLE_STRIP = C4JRender::PRIMITIVE_TYPE_TRIANGLE_STRIP;

// ─── Singleton ────────────────────────────────────────────────────────────────
extern C4JRender RenderManager;