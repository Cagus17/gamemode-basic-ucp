FUNC::SpawnTimer(playerid)
{
    if(SQL_IsCharacterLogged(playerid))
    {
        foreach(new i : Player) if(!PlayerData[i][pDisableLogin])
        {
            {
                new country[24], city[24];
                GetPlayerCountry(playerid, country, sizeof(country));
                GetPlayerCity(playerid, city, sizeof(city));

                SendClientMessageEx(i, X11_WHITE,"* "RED"%s "WHITE"is now login to the server%s.",ReturnName(playerid,1), !strcmp(country, "Unknown", true) ? ("") : sprintf("(%s, %s)", city, country));
            }
        }
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