/*

    » Basic UCP SA:MP Roleplay Gamemode
    
    » Credits: Cagus
			   
    » NOTE: Please don't remove the credits!
    
	===== » V.0.5 Changelog =====
	
	» Added Login & Register
	
	==========================
    
*/
#pragma compat 1
#pragma compress 0
#pragma dynamic 1_048_576

/* Includes */
#include <streamer>                 //by Incognito
#include <sscanf2>                  //by Y_Less fixed by maddinat0r & Emmet_
#include <a_mysql> 
#include <gvar>                     //by Incognito
#include <chrono>                   //by Southclaws
#include <crashdetect>
#include <YSI\y_timers>             //by Y_Less from YSI
#include <YSI\y_colours>            //by Y_Less from YSI
#include <strlib>                   //by Slice
#include <garage_block>             //by Crayder
#include <eSelection>
#include <lookup>                   //by Emmet_

//==========[ MODULAR ]==========
#include "../main/define.inc"
#include "../main/enum.inc"
#include "../main/color.inc"
#include "../main/mysql.inc"

//===============================
forward OnGameModeInitEx();
forward OnPlayerLogin(playerid);
forward OnPlayerDisconnectEx(playerid, reason);

/* Gamemode Start! */

main()
{
    print("\n----------------------------------------");
    print(" Gamemode Ucp versi 0.1 register dan login");
    print("----------------------------------------\n");
}

public OnGameModeInit()
{
	#if defined DEBUG_MODE
        printf("[debug] OnGameModeInit()");
	#endif

    print("[OnGameModeInit] Initialising 'Main'...");
    OnGameModeInit_Setup();
    #if defined main_OnGameModeInit
        return main_OnGameModeInit();
    #else
    return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit main_OnGameModeInit
#if defined main_OnGameModeInit
    forward main_OnGameModeInit();
#endif

#include "../main/func.inc"
#include "../main/dialog.inc"
#include "../main/timer.inc"

OnGameModeInit_Setup()
{
    //Server configuration
    MySqlStartConnection();
    ManualVehicleEngineAndLights();
    Streamer_ToggleErrorCallback(1);
    SetGameModeText(SERVER_REVISION);
    BlockGarages(true, GARAGE_TYPE_ALL, "CLOSED");

    CallLocalFunction("OnGameModeInitEx", "");
    return 1;
}

public OnGameModeExit()
{
    foreach(new playerid : Player)
        TerminateConnection(playerid);
	MySqlCloseConnection();
	return 1;
}

public OnPlayerConnect(playerid)
{
	g_RaceCheck{playerid} ++;
    ResetStatistics(playerid);
    SetPlayerColor(playerid, DEFAULT_COLOR);
	GetPlayerName(playerid, ReturnName(playerid), MAX_PLAYER_NAME + 1);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	SQL_SaveAccounts(playerid);
    TerminateConnection(playerid);
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	return 1;
}
static SetDefaultSpawn(playerid)
{
    SetPlayerPosEx(playerid, 1682.4869,-2330.1724,13.5469);
    SetPlayerFacingAngle(playerid, 359.7332);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    return 1;
}
static SetCameraData(playerid)
{
    TogglePlayerSpectating(playerid, 1);
    switch(random(1))
    {
        case 0:
        {         
            InterpolateCameraPos(playerid, 1664.715332, -2283.450927, 27.731086, 1647.487182, -2323.924560, 14.597775, 3000, 1);
            InterpolateCameraLookAt(playerid, 1661.290771, -2285.490234, 27.394948, 1643.885009, -2325.662841, 14.550904, 3000, 1);
        }
        case 1:
        {
            InterpolateCameraPos(playerid, 1664.715332, -2283.450927, 27.731086, 1647.487182, -2323.924560, 14.597775, 3000, 1);
            InterpolateCameraLookAt(playerid, 1661.290771, -2285.490234, 27.394948, 1643.885009, -2325.662841, 14.550904, 3000, 1);
        }
    }
    return 1;
}
public OnPlayerRequestClass(playerid, classid)
{
    if(IsValidRoleplayName(ReturnName(playerid)))
    {
        SendErrorMessage(playerid, "VPRP Account Center tidak sesuai format!");
        SendErrorMessage(playerid, "Penggunaan Account Center harus mengikuti format nama biasa.");
        SendErrorMessage(playerid, "Sebagai contoh, Agung, Gajahduduk, KevinSanjaya, DimasNugroho dan lainnya.");
        KickEx(playerid);
    }
    if(!PlayerData[playerid][pKicked])
    {
        SetCameraData(playerid);
        SQL_CheckAccount(playerid);
        SetPlayerColor(playerid, DEFAULT_COLOR);
    }
    return 1;
}
public OnPlayerRequestSpawn(playerid)
{
    if (AccountData[playerid][uLogged] == 0)
    {
        KickEx(playerid);
        return 0;
    }
    return 1;
}

public OnPlayerSpawn(playerid)
{
	#if defined DEBUG_MODE
	    printf("[debug] OnPlayerSpawn(PID : %d)", playerid);
	#endif
    SetPlayerScore(playerid, PlayerData[playerid][pScore]);
    Streamer_ToggleIdleUpdate(playerid, true);

    if(!PlayerData[playerid][pCreated])
    {
        for (new i = 0; i < 20; i ++) {
            SendClientMessage(playerid , -1, "");
        }
        PlayerData[playerid][pSkin] = 98;
        SetPlayerSkinEx(playerid, 98);

        PlayerData[playerid][pOrigin][0] = '\0';
        PlayerData[playerid][pBirthdate][0] = '\0';

        SendServerMessage(playerid, "Data pada karakter anda belum lengkap, isi data sesuai dengan keadaan karakter.");

        SetPlayerPos(playerid, 258.0770, -42.3550, 1002.0234);
        SetPlayerFacingAngle(playerid,45.5218);
        SetPlayerInterior(playerid, 14);
        SetPlayerVirtualWorld(playerid, (playerid+3));
        SetPlayerCameraPos(playerid,255.014175,-39.542194,1002.023437);
        SetPlayerCameraLookAt(playerid,257.987945,-42.462291,1002.023437);
    }
    else
    {
        SetPlayerFacingAngle(playerid, PlayerData[playerid][pPos][3]);

        SetPlayerInterior(playerid, PlayerData[playerid][pInterior]);
        SetPlayerVirtualWorld(playerid, PlayerData[playerid][pWorld]);
    }
	return 1;
}

ResetStatistics(playerid)
{
    PlayerData[playerid][pID] = -1;
    PlayerData[playerid][pGender] = 1;
    PlayerData[playerid][pSkin] = 98;
    PlayerData[playerid][pMoney] = 500;
    PlayerData[playerid][pBankMoney] = 1000;
    printf("Resetting player statistics for ID %d", playerid);
    return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
    return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleSirenStateChange(playerid, vehicleid, newstate)
{
    return 1;
}

public OnModelSelectionResponse(playerid, extraid, index, modelid, response)
{
	if((response) && (extraid == MODEL_SELECTION_SKIN))
    {
        for (new i = 0; i < 50; i ++) {
            SendClientMessage(playerid, -1, "");
        }

        PlayerData[playerid][pSkin] = modelid;
        PlayerData[playerid][pScore] = 1;
        PlayerData[playerid][pCreated] = 1;
        PlayerData[playerid][pHour] = 0;
        PlayerData[playerid][pMinute] = 0;
        PlayerData[playerid][pSecond] = 0;
        PlayerData[playerid][pLogged] = 1;
        SetHealth(playerid, 100);

        PlayerData[playerid][pRegisterDate] = gettime();
        mysql_tquery(sqlcon, sprintf("UPDATE `characters` SET `RegisterDate`='%d' WHERE `ID`='%d';", PlayerData[playerid][pRegisterDate], PlayerData[playerid][pID]));

        SetPlayerSkinEx(playerid, modelid);
        TogglePlayerSpectating(playerid, false);

        SetDefaultSpawn(playerid);
        SQL_SaveAccounts(playerid);

        SetPlayerScore(playerid,PlayerData[playerid][pScore]);

        TogglePlayerControllable(playerid, 0);
    }
	return 1;
}
public OnPlayerText(playerid, text[])
{
	SendNearbyMessage(playerid, 15.0, COLOR_WHITE, "%s:  %s", ReturnName(playerid, 0), text);
    return 0;
}