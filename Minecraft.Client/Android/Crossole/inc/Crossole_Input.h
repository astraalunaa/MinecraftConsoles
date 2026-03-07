#pragma once

// Crossole_Input.h
// Android input abstraction layer for MinecraftConsoles.
// Based on 4J Studios' input interface, reimplemented for Android.
// Sources: Windows64, PSVita, Durango 4J_Input headers.

#include "platform_android.h"
#include <android/input.h>

// ─── Map style constants ──────────────────────────────────────────────────────
#define MAP_STYLE_0     0
#define MAP_STYLE_1     1
#define MAP_STYLE_2     2

// ─── Button bitmask — Xbox 360 layout (used by all platforms) ────────────────
#define _360_JOY_BUTTON_A               0x00000001
#define _360_JOY_BUTTON_B               0x00000002
#define _360_JOY_BUTTON_X               0x00000004
#define _360_JOY_BUTTON_Y               0x00000008
#define _360_JOY_BUTTON_START           0x00000010
#define _360_JOY_BUTTON_BACK            0x00000020
#define _360_JOY_BUTTON_RB              0x00000040
#define _360_JOY_BUTTON_LB              0x00000080
#define _360_JOY_BUTTON_RTHUMB          0x00000100
#define _360_JOY_BUTTON_LTHUMB          0x00000200
#define _360_JOY_BUTTON_DPAD_UP         0x00000400
#define _360_JOY_BUTTON_DPAD_DOWN       0x00000800
#define _360_JOY_BUTTON_DPAD_LEFT       0x00001000
#define _360_JOY_BUTTON_DPAD_RIGHT      0x00002000
// Fake digital versions of analog stick values
#define _360_JOY_BUTTON_LSTICK_RIGHT    0x00004000
#define _360_JOY_BUTTON_LSTICK_LEFT     0x00008000
#define _360_JOY_BUTTON_RSTICK_DOWN     0x00010000
#define _360_JOY_BUTTON_RSTICK_UP       0x00020000
#define _360_JOY_BUTTON_RSTICK_RIGHT    0x00040000
#define _360_JOY_BUTTON_RSTICK_LEFT     0x00080000
#define _360_JOY_BUTTON_LSTICK_DOWN     0x00100000
#define _360_JOY_BUTTON_LSTICK_UP       0x00200000
#define _360_JOY_BUTTON_RT              0x00400000
#define _360_JOY_BUTTON_LT              0x00800000

// ─── Android button aliases — maps Android/PlayStation face button layout ─────
// Following PSVita convention of aliasing to 360 bitmasks
#define _AND_JOY_BUTTON_A               _360_JOY_BUTTON_A       // Cross / South
#define _AND_JOY_BUTTON_B               _360_JOY_BUTTON_B       // Circle / East
#define _AND_JOY_BUTTON_X               _360_JOY_BUTTON_X       // Square / West
#define _AND_JOY_BUTTON_Y               _360_JOY_BUTTON_Y       // Triangle / North
#define _AND_JOY_BUTTON_START           _360_JOY_BUTTON_START
#define _AND_JOY_BUTTON_BACK            _360_JOY_BUTTON_BACK
#define _AND_JOY_BUTTON_RB              _360_JOY_BUTTON_RB
#define _AND_JOY_BUTTON_LB              _360_JOY_BUTTON_LB
#define _AND_JOY_BUTTON_RTHUMB          _360_JOY_BUTTON_RTHUMB
#define _AND_JOY_BUTTON_LTHUMB          _360_JOY_BUTTON_LTHUMB
#define _AND_JOY_BUTTON_DPAD_UP         _360_JOY_BUTTON_DPAD_UP
#define _AND_JOY_BUTTON_DPAD_DOWN       _360_JOY_BUTTON_DPAD_DOWN
#define _AND_JOY_BUTTON_DPAD_LEFT       _360_JOY_BUTTON_DPAD_LEFT
#define _AND_JOY_BUTTON_DPAD_RIGHT      _360_JOY_BUTTON_DPAD_RIGHT
#define _AND_JOY_BUTTON_LSTICK_RIGHT    _360_JOY_BUTTON_LSTICK_RIGHT
#define _AND_JOY_BUTTON_LSTICK_LEFT     _360_JOY_BUTTON_LSTICK_LEFT
#define _AND_JOY_BUTTON_RSTICK_DOWN     _360_JOY_BUTTON_RSTICK_DOWN
#define _AND_JOY_BUTTON_RSTICK_UP       _360_JOY_BUTTON_RSTICK_UP
#define _AND_JOY_BUTTON_RSTICK_RIGHT    _360_JOY_BUTTON_RSTICK_RIGHT
#define _AND_JOY_BUTTON_RSTICK_LEFT     _360_JOY_BUTTON_RSTICK_LEFT
#define _AND_JOY_BUTTON_LSTICK_DOWN     _360_JOY_BUTTON_LSTICK_DOWN
#define _AND_JOY_BUTTON_LSTICK_UP       _360_JOY_BUTTON_LSTICK_UP
#define _AND_JOY_BUTTON_RT              _360_JOY_BUTTON_RT
#define _AND_JOY_BUTTON_LT              _360_JOY_BUTTON_LT

// ─── Stick axis maps — SouthPaw remapping support ────────────────────────────
#define AXIS_MAP_LX     0
#define AXIS_MAP_LY     1
#define AXIS_MAP_RX     2
#define AXIS_MAP_RY     3

// ─── Trigger maps ─────────────────────────────────────────────────────────────
#define TRIGGER_MAP_0   0
#define TRIGGER_MAP_1   1

// ─── Keyboard result ──────────────────────────────────────────────────────────
enum EKeyboardResult
{
	EKeyboard_Pending,
	EKeyboard_Cancelled,
	EKeyboard_ResultAccept,
	EKeyboard_ResultDecline,
};

// ─── String verify response (Xbox Live TCR stub — no-op on Android) ───────────
typedef struct _STRING_VERIFY_RESPONSE
{
	WORD     wNumStrings;
	HRESULT* pStringResult;
} STRING_VERIFY_RESPONSE;

// ─── Touch data — mirrors PSVita SceTouchData layout ─────────────────────────
// Populated from Android AInputEvent touch events in Crossole_Input.cpp
#define CROSSOLE_TOUCH_MAX_REPORT   10   // Android supports more touch points than Vita

struct CrossoleTouchReport
{
	uint16_t id;    // pointer ID
	uint16_t x;     // x position in screen pixels
	uint16_t y;     // y position in screen pixels
};

struct CrossoleTouchData
{
	uint64_t          timeStamp;
	uint32_t          status;
	uint8_t           reportNum;
	CrossoleTouchReport report[CROSSOLE_TOUCH_MAX_REPORT];
};

// ─── C_4JInput — renamed to CrossoleInput internally but typedef'd for ────────
// compatibility with existing game code that uses C_4JInput
class C_4JInput
{
public:
	// Maximum connected gamepads (from Durango)
	static const int MAX_GAMEPADS = 8;

	// Keyboard modes — superset of all platforms
	enum EKeyboardMode
	{
		EKeyboardMode_Default,
		EKeyboardMode_Numeric,
		EKeyboardMode_Password,
		EKeyboardMode_Alphabet,
		EKeyboardMode_Full,
		EKeyboardMode_Alphabet_Extended,
		EKeyboardMode_IP_Address,
		EKeyboardMode_Phone,
		EKeyboardMode_URL,      // from Durango
		EKeyboardMode_Email     // from Durango
	};

	// ── Init / tick ───────────────────────────────────────────────────────────
	void            Initialise(int iInputStateC, unsigned char ucMapC,
							   unsigned char ucActionC, unsigned char ucMenuActionC);
	void            Tick(void);

	// ── Gamepad configuration ─────────────────────────────────────────────────
	void            SetDeadzoneAndMovementRange(unsigned int uiDeadzone,
												unsigned int uiMovementRangeMax);
	void            SetGameJoypadMaps(unsigned char ucMap, unsigned char ucAction,
									  unsigned int uiActionVal);
	unsigned int    GetGameJoypadMaps(unsigned char ucMap, unsigned char ucAction);
	void            SetJoypadMapVal(int iPad, unsigned char ucMap);
	unsigned char   GetJoypadMapVal(int iPad);
	void            SetJoypadSensitivity(int iPad, float fSensitivity);
	void            SetJoypadStickAxisMap(int iPad, unsigned int uiFrom, unsigned int uiTo);
	void            SetJoypadStickTriggerMap(int iPad, unsigned int uiFrom, unsigned int uiTo);
	void            SetKeyRepeatRate(float fRepeatDelaySecs, float fRepeatRateSecs);
	void            SetDebugSequence(const char* chSequenceA,
									 int(*Func)(LPVOID), LPVOID lpParam);

	// ── Pad state ─────────────────────────────────────────────────────────────
	bool            IsPadConnected(int iPad);
	bool            IsPadLocked(int iPad);                  // from Durango
	unsigned int    GetConnectedGamepadCount();             // from Durango
	FLOAT           GetIdleSeconds(int iPad);
	void            SetMenuDisplayed(int iPad, bool bVal);

	// ── Button queries ────────────────────────────────────────────────────────
	unsigned int    GetValue(int iPad, unsigned char ucAction, bool bRepeat = false);
	bool            ButtonPressed(int iPad, unsigned char ucAction = 255);
	bool            ButtonReleased(int iPad, unsigned char ucAction);
	bool            ButtonDown(int iPad, unsigned char ucAction = 255);

	// ── Analogue axes ─────────────────────────────────────────────────────────
	float           GetJoypadStick_LX(int iPad, bool bCheckMenuDisplay = true);
	float           GetJoypadStick_LY(int iPad, bool bCheckMenuDisplay = true);
	float           GetJoypadStick_RX(int iPad, bool bCheckMenuDisplay = true);
	float           GetJoypadStick_RY(int iPad, bool bCheckMenuDisplay = true);
	unsigned char   GetJoypadLTrigger(int iPad, bool bCheckMenuDisplay = true);
	unsigned char   GetJoypadRTrigger(int iPad, bool bCheckMenuDisplay = true);

	// ── Touch (from PSVita) ───────────────────────────────────────────────────
	// Maps a screen region touch to a button bitmask entry
	void                MapTouchInput(int iPad, unsigned int uiActionVal);
	// Returns raw touch data for the given pad slot
	CrossoleTouchData*  GetTouchPadData(int iPad, bool bCheckMenuDisplay);

	// ── Circle/Cross swap (PSVita/PS4 regional button swap) ───────────────────
	void            SetCircleCrossSwapped(bool swapped);
	bool            IsCircleCrossSwapped();

	// ── Device capability queries (replaces IsVitaTV) ────────────────────────
	// Query these individually rather than assuming a device category.
	// e.g. show touch controls if HasTouchScreen(), not based on device type.
	bool            HasTouchScreen();       // false on Android TV boxes, some Chromebooks
	bool            HasPhysicalGamepad();   // true if a gamepad is currently connected
	bool            IsLargeScreen();        // true for tablets and TV form factors

	// ── Keyboard ─────────────────────────────────────────────────────────────
	// Triggers Android soft keyboard via JNI; result delivered via callback
	EKeyboardResult RequestKeyboard(LPCWSTR Title, LPCWSTR Text, DWORD dwPad,
									UINT uiMaxChars,
									int(*Func)(LPVOID, const bool),
									LPVOID lpParam,
									C_4JInput::EKeyboardMode eMode);
	void            GetText(uint16_t* UTF16String);
	void            DestroyKeyboard();                      // from Durango

	// ── String verification (Xbox Live TCR — always approved on Android) ──────
	bool            VerifyStrings(WCHAR** pwStringA, int iStringC,
								  int(*Func)(LPVOID, STRING_VERIFY_RESPONSE*),
								  LPVOID lpParam);
	void            CancelQueuedVerifyStrings(int(*Func)(LPVOID, STRING_VERIFY_RESPONSE*),
											  LPVOID lpParam);
	void            CancelAllVerifyInProgress(void);

	// ── Android lifecycle ─────────────────────────────────────────────────────
	void            Resume();   // from Durango PLM — call on Activity onResume
	void            Suspend();  // call on Activity onPause
};

// Global singleton
extern C_4JInput InputManager;