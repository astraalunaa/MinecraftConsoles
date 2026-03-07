#include <array>
#include <cstring>
#include <android/log.h>

#include "platform_android.h"
#include "../inc/Crossole_Input.h"

#define LOG_TAG "Crossole_Input"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)

// Global singleton instance
C_4JInput InputManager;

// Internal State
//  Gamepad
static std::array<uint32_t, 4> s_buttonsCurrent = {};
static std::array<uint32_t, 4> s_buttonsPrevious = {};
static std::array<float, 4> s_axisLX = {}, s_axisLY = {};
static std::array<float, 4> s_axisRX = {}, s_axisRY = {};
static std::array<uint8_t, 4> s_triggerL = {}, s_triggerR = {};
static std::array<bool, 4> s_padConnected = {false, false, false, false};
//  Touch
static std::array<CrossoleTouchData, 4> s_touchData = {};
// Virtual Keyboard
static int (*s_keyboardCallback)(LPVOID, const bool) = nullptr;

static LPVOID s_keyboardCallbackParam = nullptr;
static std::array<uint16_t, 256> s_keyboardResult = {};

void C_4JInput::Tick(void) {
    // Set current buttons to previous
    s_buttonsPrevious = s_buttonsCurrent;

    // TODO: poll Android GameActivity input queue here and fill s_buttonsCurrent,
    //       s_axisLX/LY/RX/RY, s_triggerL/R, s_touchData
}

void C_4JInput::Initialise(int iInputStateC, unsigned char ucMapC, unsigned char ucActionC,
                           unsigned char ucMenuActionC) {
    LOGI("C_4JInput::Initialise");

    // zero-ing now happens at declaration
    // memset(s_buttonsCurrent,  0, sizeof(s_buttonsCurrent));
    // memset(s_buttonsPrevious, 0, sizeof(s_buttonsPrevious));
}

// Configuration
void
C_4JInput::SetDeadzoneAndMovementRange(unsigned int uiDeadzone, unsigned int uiMovementRangeMax) {}

void C_4JInput::SetGameJoypadMaps(unsigned char ucMap, unsigned char ucAction,
                                  unsigned int uiActionVal) {}

unsigned int C_4JInput::GetGameJoypadMaps(unsigned char ucMap, unsigned char ucAction) { return 0; }

void C_4JInput::SetJoypadMapVal(int iPad, unsigned char ucMap) {}

unsigned char C_4JInput::GetJoypadMapVal(int iPad) { return 0; }

void C_4JInput::SetJoypadSensitivity(int iPad, float fSensitivity) {}

void C_4JInput::SetJoypadStickAxisMap(int iPad, unsigned int uiFrom, unsigned int uiTo) {}

void C_4JInput::SetJoypadStickTriggerMap(int iPad, unsigned int uiFrom, unsigned int uiTo) {}

void C_4JInput::SetKeyRepeatRate(float fRepeatDelaySecs, float fRepeatRateSecs) {}

void C_4JInput::SetDebugSequence(const char *chSequenceA, int(*Func)(LPVOID), LPVOID lpParam) {}

void C_4JInput::SetMenuDisplayed(int iPad, bool bVal) {}

void C_4JInput::SetCircleCrossSwapped(bool swapped) {}

bool C_4JInput::IsCircleCrossSwapped() { return false; }

bool C_4JInput::HasTouchScreen() { return false; }

bool C_4JInput::HasPhysicalGamepad() { return false; }

bool C_4JInput::IsLargeScreen() { return false; }

// ─── Button queries ───────────────────────────────────────────────────────────

unsigned int C_4JInput::GetValue(int iPad, unsigned char ucAction, bool bRepeat) {
    if (iPad < 0 || iPad >= 4) return 0;
    return s_buttonsCurrent[iPad];
}

bool C_4JInput::ButtonPressed(int iPad, unsigned char ucAction) {
    if (iPad < 0 || iPad >= 4) return false;
    // Pressed = on this frame but not last frame
    uint32_t mask = (ucAction == 255) ? 0xFFFFFFFF : (1u << ucAction);
    return (s_buttonsCurrent[iPad] & mask) && !(s_buttonsPrevious[iPad] & mask);
}

bool C_4JInput::ButtonReleased(int iPad, unsigned char ucAction) {
    if (iPad < 0 || iPad >= 4) return false;
    uint32_t mask = 1u << ucAction;
    return !(s_buttonsCurrent[iPad] & mask) && (s_buttonsPrevious[iPad] & mask);
}

bool C_4JInput::ButtonDown(int iPad, unsigned char ucAction) {
    if (iPad < 0 || iPad >= 4) return false;
    uint32_t mask = (ucAction == 255) ? 0xFFFFFFFF : (1u << ucAction);
    return (s_buttonsCurrent[iPad] & mask) != 0;
}

// ─── Analogue axes ────────────────────────────────────────────────────────────

float C_4JInput::GetJoypadStick_LX(int iPad, bool bCheckMenuDisplay) {
    return (iPad >= 0 && iPad < 4) ? s_axisLX[iPad] : 0.f;
}

float C_4JInput::GetJoypadStick_LY(int iPad, bool bCheckMenuDisplay) {
    return (iPad >= 0 && iPad < 4) ? s_axisLY[iPad] : 0.f;
}

float C_4JInput::GetJoypadStick_RX(int iPad, bool bCheckMenuDisplay) {
    return (iPad >= 0 && iPad < 4) ? s_axisRX[iPad] : 0.f;
}

float C_4JInput::GetJoypadStick_RY(int iPad, bool bCheckMenuDisplay) {
    return (iPad >= 0 && iPad < 4) ? s_axisRY[iPad] : 0.f;
}

unsigned char C_4JInput::GetJoypadLTrigger(int iPad, bool bCheckMenuDisplay) {
    return (iPad >= 0 && iPad < 4) ? s_triggerL[iPad] : 0;
}

unsigned char C_4JInput::GetJoypadRTrigger(int iPad, bool bCheckMenuDisplay) {
    return (iPad >= 0 && iPad < 4) ? s_triggerR[iPad] : 0;
}

// Misc
FLOAT C_4JInput::GetIdleSeconds(int iPad) { return 0.f; }

bool C_4JInput::IsPadConnected(int iPad) { return (iPad == 0); }  // touch always "pad 0"

// Touch
void C_4JInput::MapTouchInput(int iPad, unsigned int uiActionVal) {
    // TODO: map screen regions to button bitmask entries in s_buttonsCurrent
}

CrossoleTouchData *C_4JInput::GetTouchPadData(int iPad, bool bCheckMenuDisplay) {
    if (iPad < 0 || iPad >= 4) return nullptr;
    return &s_touchData[iPad];
}

// Convert wchar to JString
static jstring wcharToJString(JNIEnv* env, LPCWSTR wstr)
{
    if (!wstr) return env->NewStringUTF("");

    // Convert wchar_t (UTF-32 on Android) to UTF-16 for Java
    // Use the JNI NewString which takes UTF-16 directly
    // First convert UTF-32 wchar_t to UTF-16 jchar
    int len = wcslen(wstr);
    jchar* buf = new jchar[len];
    for (int i = 0; i < len; i++)
        buf[i] = (jchar)wstr[i];  // safe for BMP characters

    jstring result = env->NewString(buf, len);
    delete[] buf;
    return result;
}

// Map EKeyboardMode to Android InputType
static jint eModeToInputType(C_4JInput::EKeyboardMode eMode) {
    switch (eMode) {
        case C_4JInput::EKeyboardMode_Numeric:
            return 2; // TYPE_CLASS_NUMBER
            break;
        case C_4JInput::EKeyboardMode_Email:
            return 48; // TYPE_TEXT_VARIATION_EMAIL_ADDRESS
            break;
        case C_4JInput::EKeyboardMode_IP_Address:
        case C_4JInput::EKeyboardMode_URL:
            return 16; // TYPE_TEXT_VARIATION_URI
            break;
        case C_4JInput::EKeyboardMode_Password:
            return 128; // TYPE_TEXT_VARIATION_PASSWORD
            break;
        case C_4JInput::EKeyboardMode_Phone:
            return 3; // TYPE_CLASS_PHONE
        default:
            return 1; // TYPE_CLASS_TEXT
    }
}

// Virtual Keyboard
EKeyboardResult C_4JInput::RequestKeyboard(LPCWSTR Title, LPCWSTR Text, DWORD dwPad,
                                           UINT uiMaxChars, int(*Func)(LPVOID, const bool),
                                           LPVOID lpParam, C_4JInput::EKeyboardMode eMode) {

    // Get JNIenv
    JNIEnv *env = nullptr;
    g_javaVM->AttachCurrentThread(&env, nullptr);
    // Find Java Activity
    jclass activityClass = env->GetObjectClass(g_activity);

    // Show keyboard
    jmethodID showKeyboard = env->GetMethodID(activityClass, "showKeyboard",
                                              "(Ljava/lang/String;Ljava/lang/String;II)V");
    jstring jTitle = wcharToJString(env, Title);
    jstring jText = wcharToJString(env, Text);
    jint inputType = eModeToInputType(eMode);

    env->CallVoidMethod(g_activity, showKeyboard, jTitle, jText, (jint) uiMaxChars, inputType);

    return EKeyboard_Cancelled;
}

void C_4JInput::GetText(uint16_t *UTF16String) {
    if (UTF16String) UTF16String[0] = 0;
}

// ─── String verification (Xbox Live TCR — irrelevant on Android) ──────────────

bool C_4JInput::VerifyStrings(WCHAR **pwStringA, int iStringC,
                              int(*Func)(LPVOID, STRING_VERIFY_RESPONSE *), LPVOID lpParam) {
    // Always approved on Android — no content moderation service
    return true;
}

void C_4JInput::CancelQueuedVerifyStrings(int(*Func)(LPVOID, STRING_VERIFY_RESPONSE *),
                                          LPVOID lpParam) {}

void C_4JInput::CancelAllVerifyInProgress() {}