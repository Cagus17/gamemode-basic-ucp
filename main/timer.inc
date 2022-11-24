ClearPlayerChat(playerid, line)
{
    for (new i = 0; i < line; i ++) {
        SendClientMessage(playerid, -1,"");
    }
}

FUNC::SpawnTimer(playerid)
{
    if(SQL_IsCharacterLogged(playerid))
    {
        ClearPlayerChat(playerid, 20);
        SendServerMessage(playerid, "Selamat datang {ffff00}%s{ffffff}.", ReturnName(playerid, 1));
        AccountData[playerid][uLoginDate] = gettime();
        TogglePlayerControllable(playerid, 1);

        CallLocalFunction("OnPlayerLogin", "d", playerid);
    }
    return 1;
}

timer refuseLogin[300000](playerid)
{
    if(IsPlayerConnected(playerid))
    {
        SendServerMessage(playerid, "Anda di keluarkan dari server dikarenakan terlalu lama login ke dalam server.");
        KickEx(playerid);
    }
    return 1;
}
timer SetPlayerToUnfreeze[2000](playerid)
{
    if(SQL_IsCharacterLogged(playerid))
    {
        PlayerData[playerid][pFreeze] = 0;
        Streamer_ToggleIdleUpdate(playerid,0);
        TogglePlayerControllable(playerid, 1);
    }
    return 1;
}