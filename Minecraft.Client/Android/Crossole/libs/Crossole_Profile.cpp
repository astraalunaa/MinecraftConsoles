// 4J_Input_Android.cpp
// Stub implementation of C_4JInput for Android.
// Touch and gamepad input wired up via Android GameActivity input events.
// Replace TODO sections with real AInputEvent handling.

#include "platform_android.h"
#include "4J_Input.h"
#include <android/log.h>
#include <string.h>

#define LOG_TAG "4JInput"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)

// Global singleton instance
C_4JInput InputManager;

// ─── Internal state ───────────────────────────────────────────────────────────

static uint32_t s_buttonsCurrent[4]  = {};  // current frame button bitmask per pad
static uint32_t s_buttonsPrevious[4] = {};  // previous frame bitmask (for pressed/released)
static float    s_axisLX[4] = {}, s_axisLY[4] = {};
static float    s_axisRX[4] = {}, s_axisRY[4] = {};
static uint8_t  s_triggerL[4] = {}, s_triggerR[4] = {};
static bool     s_padConnected[4] = { true, false, false, false };  // pad 0 always present

// Touch stub state
static SceTouchData s_touchData[4] = {};

// ─── Called each frame by game loop ───────────────────────────────────────────

void C_4JInput::Tick(void)
{
    // Rotate button state
    memcpy(s_buttonsPrevious, s_buttonsCurrent, sizeof(s_buttonsCurrent));
    // TODO: poll Android GameActivity input queue here and fill s_buttonsCurrent,
    //       s_axisLX/LY/RX/RY, s_triggerL/R, s_touchData
}

// ─── Init ─────────────────────────────────────────────────────────────────────

void C_4JInput::Initialise(int iInputStateC, unsigned char ucMapC, unsigned char ucActionC, unsigned char ucMenuActionC)
{
    LOGI("C_4JInput::Initialise");
    memset(s_buttonsCurrent,  0, sizeof(s_buttonsCurrent));
    memset(s_buttonsPrevious, 0, sizeof(s_buttonsPrevious));
}

// ─── Configuration ────────────────────────────────────────────────────────────

void          C_4JInput::SetDeadzoneAndMovementRange(unsigned int uiDeadzone, unsigned int uiMovementRangeMax) {}
void          C_4JInput::SetGameJoypadMaps(unsigned char ucMap, unsigned char ucAction, unsigned int uiActionVal) {}
unsigned int  C_4JInput::GetGameJoypadMaps(unsigned char ucMap, unsigned char ucAction) { return 0; }
void          C_4JInput::SetJoypadMapVal(int iPad, unsigned char ucMap) {}
unsigned char C_4JInput::GetJoypadMapVal(int iPad) { return 0; }
void          C_4JInput::SetJoypadSensitivity(int iPad, float fSensitivity) {}
void          C_4JInput::SetJoypadStickAxisMap(int iPad, unsigned int uiFrom, unsigned int uiTo) {}
void          C_4JInput::SetJoypadStickTriggerMap(int iPad, unsigned int uiFrom, unsigned int uiTo) {}
void          C_4JInput::SetKeyRepeatRate(float fRepeatDelaySecs, float fRepeatRateSecs) {}
void          C_4JInput::SetDebugSequence(const char *chSequenceA, int(*Func)(LPVOID), LPVOID lpParam) {}
void          C_4JInput::SetMenuDisplayed(int iPad, bool bVal) {}
void          C_4JInput::SetCircleCrossSwapped(bool swapped) {}
bool          C_4JInput::IsCircleCrossSwapped() { return false; }
bool          C_4JInput::IsVitaTV() { return false; }

// ─── Button queries ───────────────────────────────────────────────────────────

unsigned int C_4JInput::GetValue(int iPad, unsigned char ucAction, bool bRepeat)
{
    if (iPad < 0 || iPad >= 4) return 0;
    return s_buttonsCurrent[iPad];
}

bool C_4JInput::ButtonPressed(int iPad, unsigned char ucAction)
{
    if (iPad < 0 || iPad >= 4) return false;
    // Pressed = on this frame but not last frame
    uint32_t mask = (ucAction == 255) ? 0xFFFFFFFF : (1u << ucAction);
    return (s_buttonsCurrent[iPad] & mask) && !(s_buttonsPrevious[iPad] & mask);
}

bool C_4JInput::ButtonReleased(int iPad, unsigned char ucAction)
{
    if (iPad < 0 || iPad >= 4) return false;
    uint32_t mask = 1u << ucAction;
    return !(s_buttonsCurrent[iPad] & mask) && (s_buttonsPrevious[iPad] & mask);
}

bool C_4JInput::ButtonDown(int iPad, unsigned char ucAction)
{
    if (iPad < 0 || iPad >= 4) return false;
    uint32_t mask = (ucAction == 255) ? 0xFFFFFFFF : (1u << ucAction);
    return (s_buttonsCurrent[iPad] & mask) != 0;
}

// ─── Analogue axes ────────────────────────────────────────────────────────────

float         C_4JInput::GetJoypadStick_LX(int iPad, bool bCheckMenuDisplay) { return (iPad>=0&&iPad<4) ? s_axisLX[iPad] : 0.f; }
float         C_4JInput::GetJoypadStick_LY(int iPad, bool bCheckMenuDisplay) { return (iPad>=0&&iPad<4) ? s_axisLY[iPad] : 0.f; }
float         C_4JInput::GetJoypadStick_RX(int iPad, bool bCheckMenuDisplay) { return (iPad>=0&&iPad<4) ? s_axisRX[iPad] : 0.f; }
float         C_4JInput::GetJoypadStick_RY(int iPad, bool bCheckMenuDisplay) { return (iPad>=0&&iPad<4) ? s_axisRY[iPad] : 0.f; }
unsigned char C_4JInput::GetJoypadLTrigger(int iPad, bool bCheckMenuDisplay) { return (iPad>=0&&iPad<4) ? s_triggerL[iPad] : 0; }
unsigned char C_4JInput::GetJoypadRTrigger(int iPad, bool bCheckMenuDisplay) { return (iPad>=0&&iPad<4) ? s_triggerR[iPad] : 0; }

// ─── Misc ─────────────────────────────────────────────────────────────────────

FLOAT C_4JInput::GetIdleSeconds(int iPad)   { return 0.f; }
bool  C_4JInput::IsPadConnected(int iPad)   { return (iPad == 0); }  // touch always "pad 0"

// ─── Touch ────────────────────────────────────────────────────────────────────

void C_4JInput::MapTouchInput(int iPad, unsigned int uiActionVal)
{
    // TODO: map screen regions to button bitmask entries in s_buttonsCurrent
}

SceTouchData* C_4JInput::GetTouchPadData(int iPad, bool bCheckMenuDisplay)
{
    if (iPad < 0 || iPad >= 4) return nullptr;
    return &s_touchData[iPad];
}

// ─── Keyboard ─────────────────────────────────────────────────────────────────

EKeyboardResult C_4JInput::RequestKeyboard(LPCWSTR Title, LPCWSTR Text, DWORD dwPad,
                                            UINT uiMaxChars, int(*Func)(LPVOID, const bool),
                                            LPVOID lpParam, C_4JInput::EKeyboardMode eMode)
{
    // TODO: trigger Android soft keyboard via JNI and call Func on result
    return EKeyboard_Cancelled;
}

void C_4JInput::GetText(uint16_t *UTF16String)
{
    if (UTF16String) UTF16String[0] = 0;
}

// ─── String verification (Xbox Live TCR — irrelevant on Android) ──────────────

bool C_4JInput::VerifyStrings(WCHAR **pwStringA, int iStringC,
                               int(*Func)(LPVOID, STRING_VERIFY_RESPONSE*), LPVOID lpParam)
{
    // Always approved on Android — no content moderation service
    return true;
}

void C_4JInput::CancelQueuedVerifyStrings(int(*Func)(LPVOID, STRING_VERIFY_RESPONSE*), LPVOID lpParam) {}
void C_4JInput::CancelAllVerifyInProgress() {}