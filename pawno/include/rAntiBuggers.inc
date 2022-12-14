/*rAntiBuggers by RogueDrifter 
-Feb the 10th 2018
-Last updated: release date.

-Callbacks: OnPlayerBugAttempt(playerid, bugcode)
_____________________________________________
bugcode 0 = AFK ghost      bugcode 1 = NPC spoof
bugcode 2 = Fake spawn     bugcode 3 = Fake connect
                bugcode 4 = cj run
_____________________________________________
*/
#if defined _RABUGGERS_INCLUDED_
    #endinput
#endif

#define _RABUGGERS_INCLUDED_

#include <a_samp>

#if !defined FILTERSCRIPT 

forward rAB_SpawnPlayer(playerid);
forward rAB_UsePlayerPedAnims();
forward rAB_TogglePlayerSpectating(playerid, toggle);

enum RAB_PLAYER_DATA
{
    bool:rAbConnected,
    bool:rAbSpawned,

    rAbPTicks
}

static 
    rAB_Player[MAX_PLAYERS][RAB_PLAYER_DATA],
    bool:rAB_PPedAnims;

#if defined OnPlayerBugAttempt
    forward OnPlayerBugAttempt(playerid, bugcode);
#endif

#if defined RAB_OnPlayerConnect
    forward RAB_OnPlayerConnect(playerid);
#endif

#if defined RAB_OnPlayerDisconnect
    forward RAB_OnPlayerDisconnect(playerid, reason);
#endif

#if defined RAB_OnPlayerGiveDamage
    forward RAB_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
#endif

#if defined RAB_OnPlayerSpawn
    forward RAB_OnPlayerSpawn(playerid);
#endif

#if defined RAB_OnPlayerDeath
    forward RAB_OnPlayerDeath(playerid, killerid, reason);
#endif

#if defined RAB_OnPlayerRequestClass
    forward RAB_OnPlayerRequestClass(playerid, classid);
#endif

#if defined RAB_OnPlayerUpdate
    forward RAB_OnPlayerUpdate(playerid);
#endif

public OnPlayerConnect(playerid)
{
    if(rAB_Player[playerid][rAbConnected])
    {
        #if defined OnPlayerBugAttempt
        OnPlayerBugAttempt(playerid, 3);
        #endif
    }

    else rAB_Player[playerid][rAbConnected] = true;
    if(IsPlayerNPC(playerid))
	{
		new rAB_PIP[17];
		GetPlayerIp(playerid, rAB_PIP, sizeof(rAB_PIP));
        new rAB_ServerIP[17];
        GetSVarString("bind", rAB_ServerIP, sizeof(rAB_ServerIP));
		if (!!strcmp(rAB_PIP, rAB_ServerIP))
		{
            #if defined OnPlayerBugAttempt
            OnPlayerBugAttempt(playerid, 1);
            #endif
		}
	}
    #if defined RAB_OnPlayerConnect
        return RAB_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

public OnPlayerSpawn(playerid)
{
    if(rAB_Player[playerid][rAbSpawned])
    {
        #if defined OnPlayerBugAttempt
        OnPlayerBugAttempt(playerid, 2);
        #endif
    }
    #if defined RAB_OnPlayerSpawn
        return RAB_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}

public OnPlayerDeath(playerid, killerid, reason)
{
    rAB_Player[playerid][rAbSpawned] = false;
    #if defined RAB_OnPlayerDeath
        return RAB_OnPlayerDeath(playerid, killerid, reason);
    #else
        return 1;
    #endif
}

public OnPlayerDisconnect(playerid, reason)
{
    rAB_Player[playerid][rAbSpawned]   = false;
    rAB_Player[playerid][rAbConnected] = false;
    #if defined RAB_OnPlayerDisconnect
        return RAB_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

public OnPlayerRequestClass(playerid, classid)
{
    rAB_Player[playerid][rAbSpawned] = false;
    #if defined RAB_OnPlayerRequestClass
        return RAB_OnPlayerRequestClass(playerid, classid);
    #else
        return 1;
    #endif
}

public OnPlayerUpdate(playerid)
{
    if(!rAB_PPedAnims)
    {
        if(GetPlayerAnimationIndex(playerid) == 1231 && rAB_ValidSkin(GetPlayerSkin(playerid)) && GetPlayerWeapon(playerid) != 46)
        {
            #if defined OnPlayerBugAttempt
            OnPlayerBugAttempt(playerid, 4);
            #endif
        }
    }
    rAB_Player[playerid][rAbPTicks] = GetTickCount();
    #if defined RAB_OnPlayerUpdate
        return RAB_OnPlayerUpdate(playerid);
    #else
        return 1;
    #endif
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
    if(rAB_IsPlayerAfk(playerid))
    {
        #if defined OnPlayerBugAttempt
        OnPlayerBugAttempt(playerid, 0);
        #endif
    }
    #if defined RAB_OnPlayerGiveDamage
        return RAB_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
    #else
        return 1;
    #endif
}

public rAB_SpawnPlayer(playerid)
{
    rAB_Player[playerid][rAbSpawned] = false;
    SpawnPlayer(playerid);
    return 1;
}

public rAB_UsePlayerPedAnims() 
{
    rAB_PPedAnims = true;
    UsePlayerPedAnims();
    return 1;
}

public rAB_TogglePlayerSpectating(playerid, toggle)
{
    if(!toggle)
    {
        rAB_Player[playerid][rAbSpawned] = false;
    }
    TogglePlayerSpectating(playerid, toggle);
    return 1;
}

static rAB_IsPlayerAfk(playerid)
    return (GetTickCount() > (rAB_Player[playerid][rAbPTicks]+2500));

static rAB_ValidSkin(skinid)
    return(1 <= skinid <= 73 || 75 <= skinid <= 311);

#if defined _ALS_OnPlayerConnect
  #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect RAB_OnPlayerConnect

#if defined _ALS_OnPlayerDisconnect
  #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect RAB_OnPlayerDisconnect

#if defined _ALS_OnPlayerSpawn
  #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn RAB_OnPlayerSpawn

#if defined _ALS_OnPlayerUpdate
  #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate RAB_OnPlayerUpdate

#if defined _ALS_OnPlayerDeath
  #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif

#define OnPlayerDeath RAB_OnPlayerDeath

#if defined _ALS_OnPlayerGiveDamage
	#undef OnPlayerGiveDamage
#else
	#define _ALS_OnPlayerGiveDamage
#endif

#define OnPlayerGiveDamage RAB_OnPlayerGiveDamage

#if defined _ALS_OnPlayerRequestClass
	#undef OnPlayerRequestClass
#else
	#define _ALS_OnPlayerRequestClass
#endif

#define OnPlayerRequestClass RAB_OnPlayerRequestClass

#if defined _ALS_TogglePlayerSpectating
  #undef TogglePlayerSpectating
#else
    #define _ALS_TogglePlayerSpectating
#endif

#define TogglePlayerSpectating rAB_TogglePlayerSpectating

#if defined _ALS_SpawnPlayer
  #undef SpawnPlayer
#else
    #define _ALS_SpawnPlayer
#endif

#define SpawnPlayer rAB_SpawnPlayer

#if defined _ALS_UsePlayerPedAnims
  #undef UsePlayerPedAnims
#else
    #define _ALS_UsePlayerPedAnims
#endif

#define UsePlayerPedAnims rAB_UsePlayerPedAnims

#else //If it's a FS

stock rAB_FTogglePlayerSpectating(playerid, toggle)
    return CallRemoteFunction("rAB_TogglePlayerSpectating", "ii", playerid, toggle);

stock rAB_FSpawnPlayer(playerid)
    return CallRemoteFunction("rAB_SpawnPlayer", "i", playerid);

stock rAB_FUsePlayerPedAnims()
    return CallRemoteFunction("rAB_UsePlayerPedAnims");

#if defined _ALS_TogglePlayerSpectating
  #undef TogglePlayerSpectating
#else
    #define _ALS_TogglePlayerSpectating
#endif

#define TogglePlayerSpectating rAB_FTogglePlayerSpectating

#if defined _ALS_SpawnPlayer
  #undef SpawnPlayer
#else
    #define _ALS_SpawnPlayer
#endif

#define SpawnPlayer rAB_FSpawnPlayer

#if defined _ALS_UsePlayerPedAnims
  #undef UsePlayerPedAnims
#else
    #define _ALS_UsePlayerPedAnims
#endif

#define UsePlayerPedAnims rAB_FUsePlayerPedAnims

#endif