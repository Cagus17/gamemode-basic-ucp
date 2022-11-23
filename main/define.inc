#define SERVER_NAME                     "Basic ucp v.0.1"
#define SERVER_URL                      "ComingSoon"
#define SERVER_DISCORD					"Coming Soon"
#define SERVER_REVISION                 "Basic ucp v.0.1"

#undef MAX_PLAYERS
#define MAX_PLAYERS 500

#define SendServerMessage(%0,%1)        SendClientMessageEx(%0, COLOR_SERVER, "SERVER: "WHITE""%1)
#define SendCustomMessage(%0,%1,%2)     SendClientMessageEx(%0, COLOR_SERVER, %1": "WHITE""%2)
#define SendSyntaxMessage(%0,%1)        SendClientMessageEx(%0, X11_GREY_80, "USAGE: "%1)
#define SendErrorMessage(%0,%1)         SendClientMessageEx(%0, X11_GREY_80, "ERROR: "%1)
#define SendAdminAction(%0,%1)          SendClientMessageEx(%0, X11_TOMATO, "ADMIN: "%1)
	
#define PermissionError(%0)				SendClientMessageEx(%0, X11_GREY_80, "ERROR: Kamu tidak diizinkan menggunakan perintah ini!")

#define MIN_VIRTUAL_WORLD				(1000000)
#define MAX_VIRTUAL_WORLD				(1200000)
#define MAX_CHARACTERS                  (3)

// Account and character variables
#define GetPlayerSQLID(%0)              PlayerData[%0][pID]
#define GetMoney(%0)                    PlayerData[%0][pMoney]
#define GetBank(%0)                     PlayerData[%0][pBankMoney]

#define ReturnAdminName(%0)             AccountData[%0][uUsername]
#define ReturnAdminRankName(%0)         AccountData[%0][uAdminRankName]
#define GetUCPSQLID(%0)                 AccountData[%0][uID]

#define NormalName(%0)                  CharacterList[%0][PlayerData[%0][pCharacter]]

#define FUNC::%0(%1) forward %0(%1); public %0(%1)