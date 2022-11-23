SQL_CheckAccount(playerid)
{
    new query[256];
    mysql_format(sqlcon, query, sizeof(query), "SELECT * FROM `accounts` WHERE `Username` = '%s' LIMIT 1;", ReturnName(playerid));
    mysql_tquery(sqlcon, query, "OnQueryFinished", "ddd", playerid, THREAD_FIND_USERNAME, g_RaceCheck[playerid]);

    return 1;
}

SQL_IsCharacterLogged(playerid)  {
    return (IsPlayerConnected(playerid) && PlayerData[playerid][pLogged]);
}

SQL_SaveAccounts(playerid)
{
    if (!AccountData[playerid][uLogged])
        return 0;

    new
        query[1024];

    format(query, sizeof(query), "UPDATE `accounts` SET `IP`='%s',`LeaveIP`='%s',`ReportPoint`='%d',`LoginDate`='%d',`AdminDutyTime`='%d',`AdminAcceptReport`='%d',`AdminDeniedReport`='%d',`AdminAcceptStuck`='%d',`AdminDeniedStuck`='%d',`AdminBanned`='%d',`AdminUnbanned`='%d',`AdminJail`='%d',`AdminAnswer`='%d' WHERE `ID` = '%d'",
        AccountData[playerid][uIP],
        AccountData[playerid][uLeaveIP],
        AccountData[playerid][uReportPoint],
        AccountData[playerid][uLoginDate],
        AccountData[playerid][uAdminDutyTime],
        AccountData[playerid][uAdminAcceptReport],
        AccountData[playerid][uAdminDeniedReport],
        AccountData[playerid][uAdminAcceptStuck],
        AccountData[playerid][uAdminDeniedStuck],
        AccountData[playerid][uAdminBanned],
        AccountData[playerid][uAdminUnbanned],
        AccountData[playerid][uAdminJail],
        AccountData[playerid][uAdminAnswer],
        AccountData[playerid][uID]
    );
    mysql_tquery(sqlcon, query);

    SQL_SaveCharacter(playerid);
    return 1;
}

SQL_SaveCharacter(playerid)
{
    if(!PlayerData[playerid][pLogged])
        return 0;

    new
        query[4098];

    if(GetPlayerState(playerid) != PLAYER_STATE_SPECTATING)
    {
        PlayerData[playerid][pWorld] = GetPlayerVirtualWorld(playerid);
        if(PlayerData[playerid][pWorld] < MIN_VIRTUAL_WORLD)
        {
            GetPlayerPos(playerid, PlayerData[playerid][pPos][0], PlayerData[playerid][pPos][1], PlayerData[playerid][pPos][2]);
            PlayerData[playerid][pInterior] = GetPlayerInterior(playerid);
        }
        else if(PlayerData[playerid][pWorld] > MIN_VIRTUAL_WORLD && PlayerData[playerid][pWorld] < MAX_VIRTUAL_WORLD)
        {
            PlayerData[playerid][pWorld] = 0;
            PlayerData[playerid][pInterior] = 0;
        }
        GetPlayerFacingAngle(playerid, PlayerData[playerid][pPos][3]);

        if(!PlayerData[playerid][pKilled] && PlayerData[playerid][pHealth] == 0.0) {
            PlayerData[playerid][pHealth] = 100.0;
        }
    }

    format(query, sizeof(query), "UPDATE `characters` SET `Created` = '%d', `Gender` = '%d', `Birthdate` = '%s', `Origin` = '%s', `Skin` = '%d', `PosX` = '%.4f', `PosY` = '%.4f', `PosZ` = '%.4f', `PosA` = '%.4f', `Health` = '%.4f', `Interior` = '%d', `World` = '%d', `Money` = '%d', `BankMoney` = '%d', `Played`='%d|%d|%d' WHERE `ID` = '%d'",
        PlayerData[playerid][pCreated],
        PlayerData[playerid][pGender],
        PlayerData[playerid][pBirthdate],
        PlayerData[playerid][pOrigin],
        PlayerData[playerid][pSkin],
        PlayerData[playerid][pPos][0],
        PlayerData[playerid][pPos][1],
        PlayerData[playerid][pPos][2],
        PlayerData[playerid][pPos][3],
        PlayerData[playerid][pHealth],
        PlayerData[playerid][pInterior],
        PlayerData[playerid][pWorld],
        PlayerData[playerid][pMoney],
        PlayerData[playerid][pBankMoney],
        PlayerData[playerid][pSecond],
        PlayerData[playerid][pMinute],
        PlayerData[playerid][pHour],
        PlayerData[playerid][pID]
    );
    mysql_tquery(sqlcon, query);
    return 1;
}

ShowCharacterMenu(playerid)
{
    new character_list[MAX_CHARACTERS * 25], character_count;

    for (new i; i < MAX_CHARACTERS; i ++) if(CharacterList[playerid][i][0] != EOS) {
        strcat(character_list, sprintf("%s\n", CharacterList[playerid][i]));
        character_count++;
    }

    if(character_count < MAX_CHARACTERS)
        strcat(character_list, "<New Character>");

    ShowPlayerDialog(playerid, DIALOG_SELECTCHAR, DIALOG_STYLE_LIST, "Character List", character_list, "Select", "Quit");
    return 1;
}

SQL_ReturnEscaped(const string[])
{
    new entry[256];
    mysql_escape_string(string, entry, sizeof(entry));
    return entry;
}

MySqlStartConnection()
{
    mysql_log(ERROR | WARNING);
    sqlcon = mysql_connect_file();

    if(mysql_errno(sqlcon) != 0) {

        new error[128];
        mysql_error(error, sizeof(error), sqlcon);
        printf("[Database] Failed! Error: [%d] %s", mysql_errno(sqlcon), error);
    }
    else
    {
        printf("[Database] Connected!");
        CallRemoteFunction("OnDBConnReady", "");
    }
    return 1;
}

MySqlCloseConnection()
{
    mysql_close(sqlcon);
    return 1;
}

cache_get_field_content(row, const field_name[], destination[], max_len = sizeof(destination))
{
    cache_get_value_name(row, field_name, destination, max_len);
}

cache_get_data(&rows, &fields)
{
    cache_get_row_count(rows);
    cache_get_field_count(fields);
}

cache_get_field_int(row, const field_name[])
{
    new val;
    cache_get_value_name_int(row, field_name, val);
    return val;
}

stock Float:cache_get_field_float(row, const field_name[])
{
    new
        str[16];

    cache_get_field_content(row, field_name, str, sizeof(str));
    return floatstr(str);
}