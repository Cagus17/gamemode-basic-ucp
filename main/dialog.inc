//=============[ DIALOG SYSTEM ]==========
public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_REGISTER)
    {
        if(!response) {
            SendServerMessage(playerid, "Gagal melakukan registrasi, anda keluar dari server.");
            KickEx(playerid);
        }
        else 
        {
            if(strlen(inputtext) < 8 || strlen(inputtext) > 32)
            {
                new str[250];
                format(str, sizeof(str), ""WHITE"Akun dengan nama "YELLOW"%s "WHITE"tidak terdaftar. Silahkan masukkan password dibawah untuk mendaftar:", ReturnName(playerid));
                ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Mendaftarkan akun", str, "Daftarkan", "Keluar");
                return 1;
            }
            for (new i; i < 64; i++)
                AccountData[playerid][uSalt][i] = random(79) + 47;

            SHA256_PassHash(inputtext, AccountData[playerid][uSalt], AccountData[playerid][uPassword], 64);
            ShowPlayerDialog(playerid, DIALOG_PASSWORD, DIALOG_STYLE_PASSWORD, "Konfirmasi password anda", ""WHITE_E"Masukkan password yang anda masukkan di kolom sebelumnya:", "Konfirmasi", "Kembali");
        }
        return 1;
    }
	if(dialogid == DIALOG_PASSWORD)
    {
        if(response)
        {
            new hash[65];
            SHA256_PassHash(inputtext, AccountData[playerid][uSalt], hash, sizeof(hash));

            if(strcmp(hash, AccountData[playerid][uPassword]))
                return ShowPlayerDialog(playerid, DIALOG_PASSWORD, DIALOG_STYLE_PASSWORD, "Konfirmasi password anda salah", ""WHITE_E"Masukkan password yang anda masukkan di kolom sebelumnya:\n\n"RED_E"ERROR"WHITE_E": Password tidak sesuai, masukkan ulang password atau anda dapat ...\n... mengubahnya dengan password baru dengan menekan opsi 'Kembali'", "Konfirmasi", "Kembali");

            AccountData[playerid][uRegisterDate] = gettime();

            GetPlayerIp(playerid, AccountData[playerid][uIP], 16);
            GetPlayerName(playerid, AccountData[playerid][uUsername], MAX_PLAYER_NAME + 1);

            new query[500];
            format(query, sizeof(query),"INSERT INTO `accounts` (`Username`, `Password`, `Salt`, `IP`, `RegisterDate`) VALUES ('%s', '%s', '%s', '%s', '%i')", AccountData[playerid][uUsername], SQL_ReturnEscaped(AccountData[playerid][uPassword]), SQL_ReturnEscaped(AccountData[playerid][uSalt]), AccountData[playerid][uIP], AccountData[playerid][uRegisterDate], ReturnName(playerid));
            // mysql_format(sqlcon, query, sizeof(query), "UPDATE `accounts` SET `Username` = '%s', `Password` = '%s', `Salt` = '%s', `IP` = '%s', `RegisterDate` = '%i', `Registered` = '1' WHERE `Username` = '%s'", AccountData[playerid][uUsername], SQL_ReturnEscaped(AccountData[playerid][uPassword]), SQL_ReturnEscaped(AccountData[playerid][uSalt]), AccountData[playerid][uIP], AccountData[playerid][uRegisterDate], ReturnName(playerid));
            mysql_query(sqlcon, query, false);

            format(query, sizeof(query), "SELECT * FROM `characters` WHERE `Username` = '%s' LIMIT %d;", ReturnName(playerid), MAX_CHARACTERS);
            mysql_tquery(sqlcon, query, "OnQueryFinished", "dd", playerid, THREAD_LIST_CHARACTERS);
        }
        else 
        {
            new str[500];
            format(str, sizeof(str), ""WHITE"Selamat datang "YELLOW"%s"WHITE".\n\nMasukkan password untuk mendaftarkan akun: (password minimal 8 sampai dengan 32 karakter)", ReturnName(playerid));
            ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Mendaftarkan akun", str, "Daftarkan", "Keluar");
        }
        return 1;
    }
    if(dialogid == DIALOG_LOGIN)
    {
        if(!response)
            return KickEx(playerid);

        if(isnull(inputtext))
        {
            new str[500];
            format(str, sizeof(str), ""WHITE_E"Selamat datang kembali "YELLOW_E"%s\n"WHITE_E"Versi Server: "GREEN_E"%s\n\n"WHITE"Anda di beri waktu "YELLOW"5 menit "WHITE"untuk login, atau akan di keluarkan dari server.\nSilahkan login dengan memasukkan password ke kolom di bawah ini\n\nWARNING: Masukkan password dengan benar, anda baru saja tidak memasukkan password.", ReturnName(playerid,0), SERVER_REVISION);
            ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "LOGIN", str, "Masuk", "Keluar");
            return 1;
        }
        new hash[65];
        SHA256_PassHash(inputtext, AccountData[playerid][uSalt], hash, sizeof(hash));

        if(strcmp(hash, AccountData[playerid][uPassword]))
        {
            if(++AccountData[playerid][uLoginAttempts] >= 3) {
                AccountData[playerid][uLoginAttempts] = 0;
                SendClientMessage(playerid, COLOR_WHITE, "Notice: Anda telah di kick karena kesalahan 3 kali memasukkan password.");
                KickEx(playerid);
            }
            else 
            {
                new str[500];
                format(str, sizeof(str), ""WHITE_E"Selamat datang kembali "YELLOW_E"%s\n"WHITE"Versi Server: "GREEN_E"%s\n\n"WHITE"Anda di beri waktu "YELLOW_E"5 menit "WHITE"untuk login, atau akan di keluarkan dari server.\nSilahkan login dengan memasukkan password ke kolom di bawah ini\n\n"LRED_E"SECURITY: "WHITE_E"Kesempatan login tersisa "YELLOW_E"%d"WHITE_E"/"RED_E"3 "WHITE_E"kali lagi.", ReturnName(playerid,0), SERVER_REVISION, AccountData[playerid][uLoginAttempts]);
                ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "LOGIN", str, "Masuk", "Keluar");
                ShowPlayerFooter(playerid, "Password salah, coba lagi.", 1000, 0);
            }
            return 1;
        }

        new
            query[128];

        AccountData[playerid][uLogged] = 1;

        format(query, sizeof(query), "SELECT `Character` FROM `characters` WHERE `Username` = '%s' LIMIT %d;", AccountData[playerid][uUsername], MAX_CHARACTERS);
        mysql_tquery(sqlcon, query, "OnQueryFinished", "dd", playerid, THREAD_LIST_CHARACTERS);

        stop AccountData[playerid][uLoginTimer];

        return 1;
    }
    if(dialogid == DIALOG_SELECTCHAR)
    {
        if (!response)
            return KickEx(playerid);

        for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++) if(AccountData[i][uUsername][0] != EOS)
        {
            if(!strcmp(AccountData[i][uUsername], ReturnName(playerid)) && i != playerid)
            {
                SendServerMessage(playerid, "Seseorang sedang login menggunakan UCP yang sama.");
                KickEx(playerid);
                return 1;
            }
        }

        if (CharacterList[playerid][listitem][0] == EOS)
            return ShowPlayerDialog(playerid, DIALOG_CREATECHAR, DIALOG_STYLE_INPUT, "Create Character", WHITE"Masukkan nama karakter, maksimal 24 karakter\n\nContoh: "YELLOW"Kimberly_Summers, Rein_Styles, Paul_Rostislav dan lainnya.", "Create", "Back");

        PlayerData[playerid][pCharacter] = listitem;
        SetPlayerName(playerid, CharacterList[playerid][listitem]);
        mysql_tquery(sqlcon, sprintf("SELECT * FROM `characters` WHERE `Character`='%s' ORDER BY `ID` ASC LIMIT 1;", CharacterList[playerid][PlayerData[playerid][pCharacter]]), "OnQueryFinished", "dd", playerid, THREAD_LOAD_CHARACTERS);
    }
    if(dialogid == DIALOG_CREATECHAR)
    {
        if (!response)
            return ShowCharacterMenu(playerid);

        if (!IsValidRoleplayName(inputtext) || strlen(inputtext) <= 3 && strlen(inputtext) > 24) {
            SendClientMessage(playerid, COLOR_WHITE, "Nama harus sesuai dengan aturan Roleplay, contoh: Kimberly_Summers, Rein_Styles, Paul_Rostislav");
            return ShowPlayerDialog(playerid, DIALOG_CREATECHAR, DIALOG_STYLE_INPUT, "Create Character", WHITE"Masukkan nama karakter, maksimal 24 karakter\n\nContoh: "YELLOW_E"Kimberly_Summers, Rein_Styles, Paul_Rostislav dan lainnya.", "Create", "Back");
        }
        mysql_tquery(sqlcon, sprintf("SELECT * FROM `characters` WHERE `Character` = '%s' LIMIT 1;", inputtext), "OnCharacterCheck", "is", playerid, inputtext);
    }
    if(dialogid == DIALOG_PERSONAL)
    {
        if(response)
        {
            switch(listitem)
            {
                case 0: 
                {
                    ShowPlayerDialog(playerid, DIALOG_GENDER, DIALOG_STYLE_LIST, "Jenis Kelamin", "Laki-laki\nPerempuan", "Select", "Cancel");
                }
                case 1: 
                {
                    ShowPlayerDialog(playerid, DIALOG_DATEBIRTH, DIALOG_STYLE_INPUT, "Tanggal Lahir", "Masukkan tanggal lahir dengan mengikuti format (DD/MM/YYYY):", "Submit", "Cancel");
                }
                case 2: 
                {
                    ShowPlayerDialog(playerid, DIALOG_ORIGIN, DIALOG_STYLE_INPUT, "Asal Karakter", "Silahkan memasukkan asal geografi dari karakter anda:", "Submit", "Cancel");
                }
                case 3:
                {
                    if(!strlen(PlayerData[playerid][pBirthdate]))
                        return SendErrorMessage(playerid, "Tanggal lahir belum di isi!.");

                    else if(!strlen(PlayerData[playerid][pOrigin]))

                        return SendErrorMessage(playerid, "Asal karakter belum di isi.");

                    else
                    {
                        switch (PlayerData[playerid][pGender])
                        {
                            case 1: ShowModelSelectionMenu(playerid, "Pilih Pakaian Karakter", MODEL_SELECTION_SKIN, g_aSpawnMaleSkins, sizeof(g_aSpawnMaleSkins), -16.0, 0.0, -55.0);
                            case 2: ShowModelSelectionMenu(playerid, "Pilih Pakaian Karakter", MODEL_SELECTION_SKIN, g_aSpawnFemaleSkins, sizeof(g_aSpawnFemaleSkins), -16.0, 0.0, -55.0);
                        }
                    }
                }
            }
        }
    }
    if(dialogid == DIALOG_GENDER)
    {
        if(!response) return ShowPlayerDialog(playerid, DIALOG_GENDER, DIALOG_STYLE_LIST, "Jenis Kelamin", "Laki-laki\nPerempuan", "Select", "Cancel");
        if(response)
        {
            PlayerData[playerid][pGender] = listitem + 1;
            switch (listitem) {
                case 0: {
                    ShowPlayerFooter(playerid, "~b~~h~Jenis Kelamin~n~~w~Anda seorang ~r~laki-laki");
                }
                case 1: {
                    ShowPlayerFooter(playerid, "~b~~h~Jenis Kelamin~n~~w~Anda seorang ~r~perempuan");
                }
            }
            PlayerData[playerid][pSkin] = (listitem) ? (233) : (98);
            SetPlayerSkinEx(playerid,PlayerData[playerid][pSkin]);
            new string[200], header[50];
            format(header, sizeof header, "Nama : {37DB45}%s", NormalName(playerid));
            format(string, sizeof string, "Jenis Kelamin\t ({37DB45}%s{ffffff})\nTanggal Lahir\t ({37DB45}None{ffffff})\nAsal Karakter\t ({37DB45}None{ffffff})\nSelesai", PlayerData[playerid][pGender] == 1 ? ("Laki-laki") : ("Perempuan"));
            ShowPlayerDialog(playerid, DIALOG_PERSONAL, DIALOG_STYLE_LIST, header, string,"select","Kembali");
        }
        else ShowPlayerDialog(playerid, DIALOG_GENDER, DIALOG_STYLE_LIST, "Jenis Kelamin", "Laki-laki\nPerempuan", "Select", "Cancel");
        return 1;
    }

    if(dialogid == DIALOG_DATEBIRTH)
    {
        if(!response) return ShowPlayerDialog(playerid, DIALOG_DATEBIRTH, DIALOG_STYLE_INPUT, "Tanggal Lahir", "Masukkan tanggal lahir dengan mengikuti format (DD/MM/YYYY):", "Submit", "Cancel");
        if(response)
        {
            new
                iDay,
                iMonth,
                iYear,
                day,
                month,
                year;

            getdate(year, month, day);

            static const
                    arrMonthDays[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

            if(sscanf(inputtext, "p</>ddd", iDay, iMonth, iYear)) {
                ShowPlayerDialog(playerid, DIALOG_DATEBIRTH, DIALOG_STYLE_INPUT, "Tanggal Lahir", "Error: Tidak sesuai format, gunakan / di setiap tanggal/bulan/tahun!\n\nMasukkan tanggal lahir dengan mengikuti format (DD/MM/YYYY):", "Submit", "Cancel");
            }
            else if(iYear < 1900 || iYear > year) {
                ShowPlayerDialog(playerid, DIALOG_DATEBIRTH, DIALOG_STYLE_INPUT, "Tanggal Lahir", "Error: Takun tidak sesuai!\n\nMasukkan tanggal lahir dengan mengikuti format (DD/MM/YYYY):", "Submit", "Cancel");
            }
            else if(iMonth < 1 || iMonth > 12) {
                ShowPlayerDialog(playerid, DIALOG_DATEBIRTH, DIALOG_STYLE_INPUT, "Tanggal Lahir", "Error: Bulan tidak sesuai!\n\nMasukkan tanggal lahir dengan mengikuti format (DD/MM/YYYY):", "Submit", "Cancel");
            }
            else if(iDay < 1 || iDay > arrMonthDays[iMonth - 1]) {
                ShowPlayerDialog(playerid, DIALOG_DATEBIRTH, DIALOG_STYLE_INPUT, "Tanggal Lahir", "Error: Hari tidak sesuai!\n\nMasukkan tanggal lahir dengan mengikuti format (DD/MM/YYYY):", "Submit", "Cancel");
            }
            else {
                format(PlayerData[playerid][pBirthdate], 24, inputtext);

                new string[200], header[50];
                format(header, sizeof header, "Nama : {37DB45}%s", NormalName(playerid));
                format(string, sizeof string, "Jenis Kelamin\t ({37DB45}%s{ffffff})\nTanggal Lahir\t ({37DB45}%s{ffffff})\nAsal Karakter\t ({37DB45}None{ffffff})\nSelesai", PlayerData[playerid][pGender] == 1 ? ("Laki-laki") : ("Perempuan"), PlayerData[playerid][pBirthdate]);
                ShowPlayerDialog(playerid, DIALOG_PERSONAL, DIALOG_STYLE_LIST, header, string,"select","Kembali");
            }
        }
        else ShowPlayerDialog(playerid, DIALOG_DATEBIRTH, DIALOG_STYLE_INPUT, "Tanggal Lahir", "Masukkan tanggal lahir dengan mengikuti format (DD/MM/YYYY):", "Submit", "Cancel");
        return 1;
    }

    if(dialogid == DIALOG_ORIGIN)
    {
        if(!response) return ShowPlayerDialog(playerid, DIALOG_ORIGIN, DIALOG_STYLE_INPUT, "Asal Karakter", "Silahkan memasukkan asal geografi dari karakter anda:", "Submit", "Cancel");
        if(response)
        {

            if(isnull(inputtext) || strlen(inputtext) <= 3 || strlen(inputtext) > 32)
                return ShowPlayerDialog(playerid, DIALOG_ORIGIN, DIALOG_STYLE_INPUT, "Asal Karakter", "Silahkan memasukkan asal geografi dari karakter anda:", "Submit", "Cancel");

            for (new i = 0, len = strlen(inputtext); i != len; i ++) {
                if((inputtext[i] >= 'A' && inputtext[i] <= 'Z') || (inputtext[i] >= 'a' && inputtext[i] <= 'z') || (inputtext[i] >= '0' && inputtext[i] <= '9') || (inputtext[i] == ' ') || (inputtext[i] == ',') || (inputtext[i] == '.'))
                    continue;

                else return ShowPlayerDialog(playerid, DIALOG_ORIGIN, DIALOG_STYLE_INPUT, "Asal Karakter", "Error: Tidak dapat memasukkan data selain huruf dan angka.\n\nSilahkan memasukkan asal geografi dari karakter anda:", "Submit", "Cancel");
            }
            format(PlayerData[playerid][pOrigin], 32, inputtext);

            new string[500], header[50];
            format(header, sizeof header, "Nama : {37DB45}%s", NormalName(playerid));
            format(string, sizeof string, "Jenis Kelamin\t ({37DB45}%s{ffffff})\nTanggal Lahir\t ({37DB45}%s{ffffff})\nAsal Karakter\t ({37DB45}%s{ffffff})\nSelesai", PlayerData[playerid][pGender] == 1 ? ("Laki-laki") : ("Perempuan"), PlayerData[playerid][pBirthdate], PlayerData[playerid][pOrigin]);
            ShowPlayerDialog(playerid, DIALOG_PERSONAL, DIALOG_STYLE_LIST, header, string,"select","Kembali");
        }
        else ShowPlayerDialog(playerid, DIALOG_ORIGIN, DIALOG_STYLE_INPUT, "Asal Karakter", "Silahkan memasukkan asal geografi dari karakter anda:", "Submit", "Cancel");
        return 1;
    }
	return 1;
}