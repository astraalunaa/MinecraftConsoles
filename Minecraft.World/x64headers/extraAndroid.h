#pragma once
// extra_android.h
// Android equivalent of extraX64.h.
// Cleaned up version with:
//   - Forward slash paths
//   - No duplicate definitions (those live in platform_android.h)
//   - No Xbox/PS3/Orbis/Vita platform blocks
//   - pthread-backed XLockFreeStack

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <functional>
#include <vector>

#include "../../Minecraft.Client/SkinBox.h"

// 'byte' is used throughout the game code as unsigned char.
// Guard against conflict with std::byte from C++17 cstddef.
typedef unsigned char byte;

// ─── Network player counts ────────────────────────────────────────────────────
// XUSER_INDEX_ANY, XUSER_MAX_COUNT etc. defined in platform_android.h

// ─── User constants ───────────────────────────────────────────────────────────
const int XUSER_INDEX_ANY  = 255;
const int XUSER_INDEX_FOCUS = 254;
const int XUSER_MAX_COUNT   = 4;
const int XUSER_NAME_SIZE   = 32;
const int INVALID_XUID      = 0;

// ─── XLockFreeStack — pthread backed ─────────────────────────────────────────
template <typename T> class XLockFreeStack
{
    std::vector<T*> intStack;
    CRITICAL_SECTION m_cs;
public:
    XLockFreeStack()  { InitializeCriticalSectionAndSpinCount(&m_cs, 5120); }
    ~XLockFreeStack() { DeleteCriticalSection(&m_cs); }
    void Initialize() {}
    void Push(T* data) {
        EnterCriticalSection(&m_cs);
        intStack.push_back(data);
        LeaveCriticalSection(&m_cs);
    }
    T* Pop() {
        EnterCriticalSection(&m_cs);
        if (intStack.size()) {
            T* ret = intStack.back();
            intStack.pop_back();
            LeaveCriticalSection(&m_cs);
            return ret;
        }
        LeaveCriticalSection(&m_cs);
        return nullptr;
    }
};

// ─── IQNetPlayer ─────────────────────────────────────────────────────────────
class IQNetPlayer
{
public:
    BYTE         GetSmallId();
    void         SendData(IQNetPlayer* player, const void* pvData, DWORD dwDataSize, DWORD dwFlags);
    bool         IsSameSystem(IQNetPlayer* player);
    DWORD        GetSendQueueSize(IQNetPlayer* player, DWORD dwFlags);
    DWORD        GetCurrentRtt();
    bool         IsHost();
    bool         IsGuest();
    bool         IsLocal();
    PlayerUID    GetXuid();
    LPCWSTR      GetGamertag();
    int          GetSessionIndex();
    bool         IsTalking();
    bool         IsMutedByLocalUser(DWORD dwUserIndex);
    bool         HasVoice();
    bool         HasCamera();
    int          GetUserIndex();
    void         SetCustomDataValue(ULONG_PTR ulpCustomDataValue);
    ULONG_PTR    GetCustomDataValue();

    BYTE      m_smallId;
    bool      m_isRemote;
    bool      m_isHostPlayer;
    PlayerUID m_resolvedXuid;
    wchar_t   m_gamertag[32];
private:
    ULONG_PTR m_customData;
};

void Win64_SetupRemoteQNetPlayer(IQNetPlayer* player, BYTE smallId, bool isHost, bool isLocal);

// ─── IQNet ────────────────────────────────────────────────────────────────────
class IQNetCallbacks {};
class IQNetGameSearch {};

typedef enum _QNET_STATE
{
    QNET_STATE_IDLE,
    QNET_STATE_SESSION_HOSTING,
    QNET_STATE_SESSION_JOINING,
    QNET_STATE_GAME_LOBBY,
    QNET_STATE_SESSION_REGISTERING,
    QNET_STATE_SESSION_STARTING,
    QNET_STATE_GAME_PLAY,
    QNET_STATE_SESSION_ENDING,
    QNET_STATE_SESSION_LEAVING,
    QNET_STATE_SESSION_DELETING
} QNET_STATE, *PQNET_STATE;

class IQNet
{
public:
    HRESULT      AddLocalPlayerByUserIndex(DWORD dwUserIndex);
    IQNetPlayer* GetHostPlayer();
    IQNetPlayer* GetLocalPlayerByUserIndex(DWORD dwUserIndex);
    IQNetPlayer* GetPlayerByIndex(DWORD dwPlayerIndex);
    IQNetPlayer* GetPlayerBySmallId(BYTE SmallId);
    IQNetPlayer* GetPlayerByXuid(PlayerUID xuid);
    DWORD        GetPlayerCount();
    QNET_STATE   GetState();
    bool         IsHost();
    HRESULT      JoinGameFromInviteInfo(DWORD dwUserIndex, DWORD dwUserMask, const INVITE_INFO* pInviteInfo);
    void         HostGame();
    void         ClientJoinGame();
    void         EndGame();

    static IQNetPlayer m_player[MINECRAFT_NET_MAX_PLAYERS];
    static DWORD       s_playerCount;
    static bool        s_isHosting;
};

// ─── PIX stubs ────────────────────────────────────────────────────────────────
inline void PIXAddNamedCounter(int, char*, ...)  {}
inline void PIXBeginNamedEvent(int, char*, ...)  {}
inline void PIXEndNamedEvent()                   {}
inline void PIXSetMarkerDeprecated(int, char*, ...) {}

// ─── Thread / handle stubs ────────────────────────────────────────────────────
typedef void* HANDLE;
inline void XSetThreadProcessor(HANDLE, int) {}

// ─── Misc network constants ───────────────────────────────────────────────────
const int QNET_SENDDATA_LOW_PRIORITY = 0;
const int QNET_SENDDATA_SECONDARY    = 0;

// ─── Content / marketplace stubs ─────────────────────────────────────────────
const int XCONTENT_MAX_DISPLAYNAME_LENGTH = 256;
const int XCONTENT_MAX_FILENAME_LENGTH    = 256;
typedef int XCONTENTDEVICEID;

typedef struct _XCONTENT_DATA {
    XCONTENTDEVICEID DeviceID;
    DWORD            dwContentType;
    WCHAR            szDisplayName[XCONTENT_MAX_DISPLAYNAME_LENGTH];
    CHAR             szFileName[XCONTENT_MAX_FILENAME_LENGTH];
} XCONTENT_DATA, *PXCONTENT_DATA;

static const int XMARKETPLACE_CONTENT_ID_LEN = 4;

typedef struct _XMARKETPLACE_CONTENTOFFER_INFO {
    ULONGLONG qwOfferID, qwPreviewOfferID;
    DWORD     dwOfferNameLength;
    WCHAR*    wszOfferName;
    DWORD     dwOfferType;
    BYTE      contentId[XMARKETPLACE_CONTENT_ID_LEN];
    BOOL      fIsUnrestrictedLicense;
    DWORD     dwLicenseMask, dwTitleID, dwContentCategory;
    DWORD     dwTitleNameLength;
    WCHAR*    wszTitleName;
    BOOL      fUserHasPurchased;
    DWORD     dwPackageSize, dwInstallSize, dwSellTextLength;
    WCHAR*    wszSellText;
    DWORD     dwAssetID, dwPurchaseQuantity, dwPointsPrice;
} XMARKETPLACE_CONTENTOFFER_INFO, *PXMARKETPLACE_CONTENTOFFER_INFO;

typedef enum {
    XMARKETPLACE_OFFERING_TYPE_CONTENT    = 0x00000002,
    XMARKETPLACE_OFFERING_TYPE_GAME_DEMO  = 0x00000020,
    XMARKETPLACE_OFFERING_TYPE_GAME_TRAILER = 0x00000040,
    XMARKETPLACE_OFFERING_TYPE_THEME      = 0x00000080,
    XMARKETPLACE_OFFERING_TYPE_TILE       = 0x00000800,
    XMARKETPLACE_OFFERING_TYPE_ARCADE     = 0x00002000,
    XMARKETPLACE_OFFERING_TYPE_VIDEO      = 0x00004000,
    XMARKETPLACE_OFFERING_TYPE_CONSUMABLE = 0x00010000,
    XMARKETPLACE_OFFERING_TYPE_AVATARITEM = 0x00100000
} XMARKETPLACE_OFFERING_TYPE;

// ─── Session / friend structs ─────────────────────────────────────────────────
typedef struct _XOVERLAPPED {} XOVERLAPPED, *PXOVERLAPPED;
typedef struct _XSESSION_SEARCHRESULT {} XSESSION_SEARCHRESULT, *PXSESSION_SEARCHRESULT;

typedef struct {
    DWORD dwContextId;
    DWORD dwValue;
} XUSER_CONTEXT, *PXUSER_CONTEXT;

typedef struct _XSESSION_SEARCHRESULT_HEADER {
    DWORD                  dwSearchResults;
    XSESSION_SEARCHRESULT* pResults;
} XSESSION_SEARCHRESULT_HEADER, *PXSESSION_SEARCHRESULT_HEADER;

typedef struct _XONLINE_FRIEND {
    PlayerUID xuid;
    CHAR      szGamertag[XUSER_NAME_SIZE];
    DWORD     dwFriendState;
    SessionID sessionID;
    DWORD     dwTitleID;
    FILETIME  ftUserTime;
    SessionID xnkidInvite;
    FILETIME  gameinviteTime;
    DWORD     cchRichPresence;
} XONLINE_FRIEND, *PXONLINE_FRIEND;

typedef struct {
    BYTE  bFlags, bReserved;
    WORD  cProbesXmit, cProbesRecv, cbData;
    BYTE* pbData;
    WORD  wRttMinInMsecs, wRttMedInMsecs;
    DWORD dwUpBitsPerSec, dwDnBitsPerSec;
} XNQOSINFO;

typedef struct {
    UINT     cxnqos, cxnqosPending;
    XNQOSINFO axnqosinfo[1];
} XNQOS;

typedef struct _XUSER_SIGNIN_INFO {
    PlayerUID xuid;
    DWORD     dwGuestNumber;
} XUSER_SIGNIN_INFO2, *PXUSER_SIGNIN_INFO2;

// ─── XUSER_DATA ───────────────────────────────────────────────────────────────
typedef struct {
    BYTE type;
    union {
        LONG      nData;
        LONGLONG  i64Data;
        double    dblData;
        struct { DWORD cbData; LPWSTR pwszData; } string;
        float     fData;
        struct { DWORD cbData; PBYTE  pbData;  } binary;
        FILETIME  ftData;
    };
} XUSER_DATA, *PXUSER_DATA;

typedef struct {
    DWORD      dwPropertyId;
    XUSER_DATA value;
} XUSER_PROPERTY, *PXUSER_PROPERTY;

// ─── XRNM send buffer ─────────────────────────────────────────────────────────
struct XRNM_SEND_BUFFER {
    DWORD dwDataSize;
    byte* pbyData;
};

// ─── VK_PAD constants ────────────────────────────────────────────────────────
#define VK_PAD_A                0x5800
#define VK_PAD_B                0x5801
#define VK_PAD_X                0x5802
#define VK_PAD_Y                0x5803
#define VK_PAD_RSHOULDER        0x5804
#define VK_PAD_LSHOULDER        0x5805
#define VK_PAD_LTRIGGER         0x5806
#define VK_PAD_RTRIGGER         0x5807
#define VK_PAD_DPAD_UP          0x5810
#define VK_PAD_DPAD_DOWN        0x5811
#define VK_PAD_DPAD_LEFT        0x5812
#define VK_PAD_DPAD_RIGHT       0x5813
#define VK_PAD_START            0x5814
#define VK_PAD_BACK             0x5815
#define VK_PAD_LTHUMB_PRESS     0x5816
#define VK_PAD_RTHUMB_PRESS     0x5817
#define VK_PAD_LTHUMB_UP        0x5820
#define VK_PAD_LTHUMB_DOWN      0x5821
#define VK_PAD_LTHUMB_RIGHT     0x5822
#define VK_PAD_LTHUMB_LEFT      0x5823
#define VK_PAD_LTHUMB_UPLEFT    0x5824
#define VK_PAD_LTHUMB_UPRIGHT   0x5825
#define VK_PAD_LTHUMB_DOWNRIGHT 0x5826
#define VK_PAD_LTHUMB_DOWNLEFT  0x5827
#define VK_PAD_RTHUMB_UP        0x5830
#define VK_PAD_RTHUMB_DOWN      0x5831
#define VK_PAD_RTHUMB_RIGHT     0x5832
#define VK_PAD_RTHUMB_LEFT      0x5833
#define VK_PAD_RTHUMB_UPLEFT    0x5834
#define VK_PAD_RTHUMB_UPRIGHT   0x5835
#define VK_PAD_RTHUMB_DOWNRIGHT 0x5836
#define VK_PAD_RTHUMB_DOWNLEFT  0x5837

// ─── Language / locale constants ─────────────────────────────────────────────
const int XC_LANGUAGE_ENGLISH    = 0x01;
const int XC_LANGUAGE_JAPANESE   = 0x02;
const int XC_LANGUAGE_GERMAN     = 0x03;
const int XC_LANGUAGE_FRENCH     = 0x04;
const int XC_LANGUAGE_SPANISH    = 0x05;
const int XC_LANGUAGE_ITALIAN    = 0x06;
const int XC_LANGUAGE_KOREAN     = 0x07;
const int XC_LANGUAGE_TCHINESE   = 0x08;
const int XC_LANGUAGE_PORTUGUESE = 0x09;
const int XC_LANGUAGE_POLISH     = 0x0B;
const int XC_LANGUAGE_RUSSIAN    = 0x0C;
const int XC_LANGUAGE_SWEDISH    = 0x0D;
const int XC_LANGUAGE_TURKISH    = 0x0E;
const int XC_LANGUAGE_BNORWEGIAN = 0x0F;
const int XC_LANGUAGE_DUTCH      = 0x10;
const int XC_LANGUAGE_SCHINESE   = 0x11;
const int XC_LANGUAGE_FINISH     = 0xF1;
const int XC_LANGUAGE_GREEK      = 0xF2;
const int XC_LANGUAGE_DANISH     = 0xF3;
const int XC_LANGUAGE_CZECH      = 0xF4;
const int XC_LANGUAGE_SLOVAK     = 0xF5;

const int XC_LOCALE_AUSTRALIA          = 1;
const int XC_LOCALE_AUSTRIA            = 2;
const int XC_LOCALE_BELGIUM            = 3;
const int XC_LOCALE_BRAZIL             = 4;
const int XC_LOCALE_CANADA             = 5;
const int XC_LOCALE_CHILE              = 6;
const int XC_LOCALE_CHINA              = 7;
const int XC_LOCALE_COLOMBIA           = 8;
const int XC_LOCALE_CZECH_REPUBLIC     = 9;
const int XC_LOCALE_DENMARK            = 10;
const int XC_LOCALE_FINLAND            = 11;
const int XC_LOCALE_FRANCE             = 12;
const int XC_LOCALE_GERMANY            = 13;
const int XC_LOCALE_GREECE             = 14;
const int XC_LOCALE_HONG_KONG          = 15;
const int XC_LOCALE_HUNGARY            = 16;
const int XC_LOCALE_INDIA              = 17;
const int XC_LOCALE_IRELAND            = 18;
const int XC_LOCALE_ITALY              = 19;
const int XC_LOCALE_JAPAN              = 20;
const int XC_LOCALE_KOREA              = 21;
const int XC_LOCALE_MEXICO             = 22;
const int XC_LOCALE_NETHERLANDS        = 23;
const int XC_LOCALE_NEW_ZEALAND        = 24;
const int XC_LOCALE_NORWAY             = 25;
const int XC_LOCALE_POLAND             = 26;
const int XC_LOCALE_PORTUGAL           = 27;
const int XC_LOCALE_SINGAPORE          = 28;
const int XC_LOCALE_SLOVAK_REPUBLIC    = 29;
const int XC_LOCALE_SOUTH_AFRICA       = 30;
const int XC_LOCALE_SPAIN              = 31;
const int XC_LOCALE_SWEDEN             = 32;
const int XC_LOCALE_SWITZERLAND        = 33;
const int XC_LOCALE_TAIWAN             = 34;
const int XC_LOCALE_GREAT_BRITAIN      = 35;
const int XC_LOCALE_UNITED_STATES      = 36;
const int XC_LOCALE_RUSSIAN_FEDERATION = 37;
const int XC_LOCALE_WORLD_WIDE         = 38;
const int XC_LOCALE_TURKEY             = 39;
const int XC_LOCALE_ARGENTINA          = 40;
const int XC_LOCALE_SAUDI_ARABIA       = 41;
const int XC_LOCALE_ISRAEL             = 42;
const int XC_LOCALE_UNITED_ARAB_EMIRATES = 43;
const int XC_LOCALE_LATIN_AMERICA      = 240;

using namespace std;