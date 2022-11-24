//==========[ Func ]==========
FUNC::OnQueryFinished(extraid, threadid, race_check)
{
    if(!IsPlayerConnected(extraid))
        return 0;

    new rows, fields;
    switch (threadid)
    {
        case THREAD_CREATE_CHAR:
        {
            if (GetPVarInt(extraid, "Verifikasi") == 1) {
                new query[128];

                SetPVarInt(extraid, "Verifikasi", 0);
                AccountData[extraid][uLogged] = 1;

                stop AccountData[extraid][uLoginTimer];

                format(query, sizeof(query), "SELECT `Character` FROM `characters` WHERE `Username` = '%s' LIMIT %d;", ReturnName(extraid), MAX_CHARACTERS);
                mysql_tquery(sqlcon, query, "OnQueryFinished", "dd", extraid, THREAD_LIST_CHARACTERS);
            }
        }
        case THREAD_LIST_CHARACTERS: {
            for (new i = 0; i < MAX_CHARACTERS; i ++) {
                CharacterList[extraid][i][0] = EOS;
            }

            for (new i = 0; i < cache_num_rows(); i ++) {
                cache_get_field_content(i, "Character", CharacterList[extraid][i], MAX_PLAYER_NAME + 1);
            }
            ShowCharacterMenu(extraid);
        }
        case THREAD_LOAD_CHARACTERS:
        {
            cache_get_data(rows, fields);

            if(rows)
            {
                new query[128];

                PlayerData[extraid][pID] = cache_get_field_int(0, "ID");
                PlayerData[extraid][pGender] = cache_get_field_int(0, "Gender");
                PlayerData[extraid][pCreated] = cache_get_field_int(0, "Created");

                cache_get_field_content(0, "Played", query, 64);
                sscanf(query, "p<|>ddd", PlayerData[extraid][pSecond], PlayerData[extraid][pMinute], PlayerData[extraid][pHour]);

                cache_get_field_content(0, "Birthdate", PlayerData[extraid][pBirthdate], 24);
                cache_get_field_content(0, "Origin", PlayerData[extraid][pOrigin], 32);
                cache_get_field_content(0, "Alias", PlayerData[extraid][pAlias], 25);

                new
                    bool:is_alias_null
                ;
                cache_is_value_name_null(0, "Alias", is_alias_null);

                if (is_alias_null)
                {
                    format(PlayerData[extraid][pAlias], 25, "Player");
                }
                else
                {
                    cache_get_field_content(0, "Alias", PlayerData[extraid][pAlias], 25);
                }

                PlayerData[extraid][pSkin] = cache_get_field_int(0, "Skin");
                PlayerData[extraid][pPos][0] = cache_get_field_float(0, "PosX");
                PlayerData[extraid][pPos][1] = cache_get_field_float(0, "PosY");
                PlayerData[extraid][pPos][2] = cache_get_field_float(0, "PosZ");
                PlayerData[extraid][pPos][3] = cache_get_field_float(0, "PosA");
                PlayerData[extraid][pHealth] = cache_get_field_float(0, "Health");
                PlayerData[extraid][pInterior] = cache_get_field_int(0, "Interior");
                PlayerData[extraid][pRegisterDate] = cache_get_field_int(0, "RegisterDate");
                PlayerData[extraid][pWorld] = cache_get_field_int(0, "World");
                PlayerData[extraid][pMoney] = cache_get_field_int(0, "Money");
                PlayerData[extraid][pBankMoney] = cache_get_field_int(0, "BankMoney");

                PlayerData[extraid][pLogged] = 1;
                PlayerData[extraid][pScore] = cache_get_field_int(0, "pScore");

                if(!PlayerData[extraid][pCreated])
                {
                    new string[200], header[50];
                    format(header, sizeof header, "Nama : {37DB45}%s", NormalName(extraid));
                    format(string, sizeof string, "Jenis Kelamin\t ({37DB45}None{ffffff})\nTanggal Lahir\t ({37DB45}None{ffffff})\nAsal Karakter\t ({37DB45}None{ffffff})\nSelesai");
                    ShowPlayerDialog(extraid, DIALOG_PERSONAL, DIALOG_STYLE_LIST, header, string,"select","Kembali");


                    SetSpawnInfo(extraid, 0, 98, 258.0770, -42.3550, 1002.0234, 0.0, 0, 0, 0, 0, 0, 0);
                    TogglePlayerSpectating(extraid, 0);
                    TogglePlayerControllable(extraid, 0);
                }
                else
                {
                    SetSpawnInfo(extraid, 0, PlayerData[extraid][pSkin], PlayerData[extraid][pPos][0], PlayerData[extraid][pPos][1], PlayerData[extraid][pPos][2], 0.0, 0, 0, 0, 0, 0, 0);

                    TogglePlayerSpectating(extraid, 0);
                    TogglePlayerControllable(extraid, 0);

                    SetTimerEx("SpawnTimer", 1000, false, "d", extraid);
                }
            }
        }
        case THREAD_FIND_USERNAME:
        {
            new string[MAX_PLAYER_NAME+1];
            if (race_check != g_RaceCheck[extraid])
                return KickEx(extraid);

            format(string, sizeof(string), "Basic - %s", ReturnName(extraid));

            if(cache_num_rows()) {
                GetPlayerName(extraid, AccountData[extraid][uUsername], MAX_PLAYER_NAME + 1);

                AccountData[extraid][uID] = cache_get_field_int(0, "ID");
                AccountData[extraid][uAdmin] = cache_get_field_int(0, "Admin");
                AccountData[extraid][uAdminHide] = cache_get_field_int(0, "AdminHide");
                //Admin Activity
                AccountData[extraid][uAdminDutyTime] = cache_get_field_int(0, "AdminDutyTime");
                AccountData[extraid][uAdminAcceptReport] = cache_get_field_int(0, "AdminAcceptReport");
                AccountData[extraid][uAdminDeniedReport] = cache_get_field_int(0, "AdminDeniedReport");
                AccountData[extraid][uAdminAcceptStuck] = cache_get_field_int(0, "AdminAcceptStuck");
                AccountData[extraid][uAdminDeniedStuck] = cache_get_field_int(0, "AdminDeniedStuck");
                AccountData[extraid][uAdminBanned] = cache_get_field_int(0, "AdminBanned");
                AccountData[extraid][uAdminUnbanned] = cache_get_field_int(0, "AdminUnbanned");
                AccountData[extraid][uAdminJail] = cache_get_field_int(0, "AdminJail");
                AccountData[extraid][uAdminAnswer] = cache_get_field_int(0, "AdminAnswer");

                AccountData[extraid][uLoginDate] = cache_get_field_int(0, "LoginDate");
                AccountData[extraid][uRegisterDate] = cache_get_field_int(0, "RegisterDate");

                cache_get_field_content(0, "IP", AccountData[extraid][uIP], 17);
                cache_get_field_content(0, "Salt", AccountData[extraid][uSalt], 65);
                cache_get_field_content(0, "Password", AccountData[extraid][uPassword], 65);
                cache_get_field_content(0, "AdminRankName", AccountData[extraid][uAdminRankName], 32);
                
                AccountData[extraid][uLoginTimer] = defer refuseLogin(extraid);
                new str[500];
                format(str, sizeof(str), ""WHITE_E"Selamat datang kembali "YELLOW_E"%s.\n"WHITE_E"Versi Server: "GREEN_E"%s\n\n"WHITE"Anda di beri waktu "YELLOW_E"5 menit "WHITE_E"untuk login, atau akan di keluarkan dari server.\nSilahkan login dengan memasukkan password ke kolom di bawah ini", ReturnName(extraid,0), SERVER_REVISION);
                ShowPlayerDialog(extraid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "LOGIN", str, "Masuk", "Keluar");
            }
            else {
                new str[500];
                format(str, sizeof(str), ""WHITE_E"Selamat datang di Basic Roleplay "YELLOW_E"%s"WHITE_E".\n\nMasukkan password untuk mendaftarkan akun: (password minimal 8 sampai dengan 32 karakter).", ReturnName(extraid));
                ShowPlayerDialog(extraid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "REGISTER", str, "Daftarkan", "Keluar");
            }
        }
    }
    return 1;
}
KickEx(playerid, time = 200)
{
    if(PlayerData[playerid][pKicked])
        return 0;

    if(SQL_IsCharacterLogged(playerid)) {
        SQL_SaveAccounts(playerid);
    }

    PlayerData[playerid][pKicked] = 1;
    SetTimerEx("KickTimer", time, false, "d", playerid);
    return 1;
}

ReturnName(playerid, underscore=1, mask = 0)
{
    new
        name[MAX_PLAYER_NAME + 1];

    GetPlayerName(playerid, name, sizeof(name));

    if(!underscore) {
        for (new i = 0, len = strlen(name); i < len; i ++) {
                if(name[i] == '_') name[i] = ' ';
        }
    }

    if(mask){
        if(PlayerData[playerid][pMaskOn] && !AccountData[playerid][uAdminDuty])
            format(name, sizeof(name), "Mask_#%d", PlayerData[playerid][pMaskID]);
    }
    return name;
}

SendNearbyMessage(playerid, Float:radius, color, const str[], {Float,_}:...)
{
    static
        args,
        start,
        end,
        string[144]
    ;
    #emit LOAD.S.pri 8
    #emit STOR.pri args

    if(args > 16)
    {
        #emit ADDR.pri str
        #emit STOR.pri start

        for (end = start + (args - 16); end > start; end -= 4)
        {
            #emit LREF.pri end
            #emit PUSH.pri
        }
        #emit PUSH.S str
        #emit PUSH.C 144
        #emit PUSH.C string

        #emit LOAD.S.pri 8
        #emit CONST.alt 4
        #emit SUB
        #emit PUSH.pri

        #emit SYSREQ.C format
        #emit LCTRL 5
        #emit SCTRL 4

        foreach (new i : Player)
        {
            if(IsPlayerNearPlayer(i, playerid, radius)) {
                SendClientMessage(i, color, string);
            }
        }
        return 1;
    }
    foreach (new i : Player)
    {
        if(IsPlayerNearPlayer(i, playerid, radius)) {
            SendClientMessage(i, color, str);
        }
    }
    return 1;
}

stock IsPlayerNearPlayer(playerid, targetid, Float:radius)
{
    static
        Float:fX,
        Float:fY,
        Float:fZ;

    GetPlayerPos(targetid, fX, fY, fZ);

    return (GetPlayerInterior(playerid) == GetPlayerInterior(targetid) && GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(targetid)) && IsPlayerInRangeOfPoint(playerid, radius, fX, fY, fZ);
}

stock SendClientMessageEx(playerid, color, const text[], {Float, _}:...)
{
    static
        args,
        str[144];

    if((args = numargs()) == 3)
    {
        SendClientMessage(playerid, color, text);
    }
    else
    {
        while (--args >= 3)
        {
            #emit LCTRL 5
            #emit LOAD.alt args
            #emit SHL.C.alt 2
            #emit ADD.C 12
            #emit ADD
            #emit LOAD.I
            #emit PUSH.pri
        }
        #emit PUSH.S text
        #emit PUSH.C 144
        #emit PUSH.C str
        #emit PUSH.S 8
        #emit SYSREQ.C format
        #emit LCTRL 5
        #emit SCTRL 4

        SendClientMessage(playerid, color, str);

        #emit RETN
    }
    return 1;
}

ShowPlayerFooter(playerid, string[], time = 1500, sound = 0) {
    if(PlayerData[playerid][pShowFooter]) {
        PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][textdraw_footer]);
        KillTimer(PlayerData[playerid][pFooterTimer]);
    }
    PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][textdraw_footer], string);
    PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][textdraw_footer]);
    PlayerData[playerid][pShowFooter] = true;
    PlayerData[playerid][pFooterTimer] = SetTimerEx("HidePlayerFooter", time, false, "d", playerid);

    if(sound) PlayerPlaySoundEx(playerid, 1085, 1);
    return 1;
}

FUNC::HidePlayerFooter(playerid) {

    if(!PlayerData[playerid][pShowFooter])
        return 0;

    PlayerData[playerid][pShowFooter] = false;
    KillTimer(PlayerData[playerid][pFooterTimer]);
    PlayerData[playerid][pFooterTimer] = 0;
    return PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][textdraw_footer]);
}

IsValidRoleplayName(const name[]) {
    if(!name[0] || strfind(name, "_") == -1)
        return 0;

    else for (new i = 0, len = strlen(name); i != len; i ++) {
    if((i == 0) && (name[i] < 'A' || name[i] > 'Z'))
            return 0;

        else if((i != 0 && i < len  && name[i] == '_') && (name[i + 1] < 'A' || name[i + 1] > 'Z'))
            return 0;

        else if((name[i] < 'A' || name[i] > 'Z') && (name[i] < 'a' || name[i] > 'z') && name[i] != '_' && name[i] != '.')
            return 0;
    }
    return 1;
}
FUNC::OnCharacterCheck(playerid, charaname[])
{
    if (!cache_num_rows())
    {
        new create_char[200];

        mysql_format(sqlcon, create_char, sizeof(create_char), "INSERT INTO `characters` (`Username`, `Character`, `CreateDate`) VALUES ('%s', '%s', '%i')", AccountData[playerid][uUsername], charaname, gettime());
        mysql_tquery(sqlcon, create_char);

        for (new i; i < MAX_CHARACTERS; i ++) if(CharacterList[playerid][i][0] == EOS) {
            strcat(CharacterList[playerid][i], charaname);
            break;
        }

        ShowCharacterMenu(playerid);
        SendServerMessage(playerid, "Karakter baru telah dibuat, kami akan memuat ulang semua karaktermu.");
        return 1;
    }

    SendClientMessage(playerid, COLOR_WHITE, "Nama tersebut sudah terpakai! Silahkan ganti nama yang lain.");
    ShowPlayerDialog(playerid, DIALOG_CREATECHAR, DIALOG_STYLE_INPUT, "Create Character","Masukkan nama karakter, maksimal 24 karakter\n\nContoh: "YELLOW_E"Kimberly_Summers, Rein_Styles, Paul_Rostislav dan lainnya.", "Create", "Back");
    return 1;
}
PlayerPlaySoundEx(playerid, sound, forall = 0)
{
    new
        Float:x,
        Float:y,
        Float:z;

    GetPlayerPos(playerid, x, y, z);

    if(forall) return PlayerPlaySound(playerid, sound, x, y, z);

    foreach (new i : Player) if(IsPlayerInRangeOfPoint(i, 20.0, x, y, z)) {
        PlayerPlaySound(i, sound, x, y, z);
    }
    return 1;
}

stock SetPlayerSkinEx(playerid, skin, choice = 0, update = 0)
{
    if(!update) {
        if(!choice) PlayerData[playerid][pSkin] = skin;
    }
    SetPlayerSkin(playerid,skin);
    return 1;
}
stock SetHealth(playerid, Float:health)
{
    PlayerData[playerid][pHealth] = health;

    if(PlayerData[playerid][pHealth] > 100)
    {
        PlayerData[playerid][pHealth] = 100.0;
        SetPlayerHealth(playerid, 100);
    }
    else if(PlayerData[playerid][pHealth] <= 5)
    {
        PlayerData[playerid][pHealth] = 1;
        SetPlayerHealth(playerid, 1);
    }
    else
    {
        SetPlayerHealth(playerid, PlayerData[playerid][pHealth]);
    }
    return 1;
}

TerminateConnection(playerid)
{
    if(PlayerData[playerid][pShowFooter]) {
        KillTimer(PlayerData[playerid][pFooterTimer]);
    }
    if(PlayerData[playerid][pFreeze]) {
        stop PlayerData[playerid][pFreezeTimer];
    }
    stop AccountData[playerid][uLoginTimer];


    SQL_SaveCharacter(playerid);
    SQL_SaveAccounts(playerid);
    ResetStatistics(playerid);
    return 1;
}

stock SetPlayerPosEx(playerid, Float:x, Float:y, Float:z, time = 500)
{
    if(PlayerData[playerid][pFreeze])
    {
        stop PlayerData[playerid][pFreezeTimer];
        PlayerData[playerid][pFreeze] = 0;
        TogglePlayerControllable(playerid, 1);
    }
    Streamer_ToggleIdleUpdate(playerid,1);
    TogglePlayerControllable(playerid, 0);
    Streamer_UpdateEx(playerid, x, y, z);
    SetCameraBehindPlayer(playerid);
    PlayerData[playerid][pFreeze] = 1;
    SetPlayerPos(playerid, x, y, z + 0.5);
    PlayerData[playerid][pFreezeTimer] = defer SetPlayerToUnfreeze[time](playerid);
    return 1;
}