/****************************************************************************
 * Copyright (C) 2009-2013 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include <ogcsys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Settings.h"
#include "FileOperations/fileops.h"
#include "DeviceControls/DeviceHandler.hpp"
#include "Language/gettext.h"
#include "Tools/StringTools.h"
#include "Tools/tools.h"
#include "Tools/encrypt.h"

#define DEFAULT_APP_PATH	"apps/WiiXplorer/"
#define CONFIGPATH			DEFAULT_APP_PATH
#define CONFIGNAME			"WiiXplorer.cfg"
#define LANGPATH			"Languages/"

CSettings Settings;

CSettings::CSettings()
{
	strcpy(BootDevice, "sd:/");
	snprintf(ConfigPath, sizeof(ConfigPath), "%s%s%s", BootDevice, CONFIGPATH, CONFIGNAME);
	this->SetDefault();
}

CSettings::~CSettings()
{
}

void CSettings::SetDefault()
{
	BootIOS = 58;
	MusicVolume = 80;
	CurrentSMBUser = 0;
	CurrentFTPUser = 0;
	CurrentNFSUser = 0;
	BGMLoopMode = 1;
	SlideshowDelay = 4;
	ImageFadeSpeed = 20;
	KeyboardDeleteDelay = 15;
	AutoConnect = 0;
	UpdateMetaxml = 1;
	UpdateIconpng = 1;
	ClockMode = 0;
	ScrollSpeed = 5;
	BrowserMode = ICONBROWSER;
	ScreenshotFormat = 0;
	SoundblockCount = 32;
	SoundblockSize = 8192;
	LoadMusicToMem = 0;
	DeleteTempPath = 1;
	CopyThreadPrio = 70;
	CopyThreadBackPrio = 30;
	Rumble = 0;
	HideSystemFiles = 1;
	ShowFormatter = 0;
	CompressionLevel = -1;
	USBPort = 0;
	MountISFS = ON;
	ISFSWriteAccess = OFF;
	ResampleTo48kHz = ON;
	OverridePriiloader = ON;
	TooltipDelay = 1000;
	PDFLoadZoom = 1.0f;
	PointerSpeed = 0.18f;
	sprintf(CustomFontPath, "%s%sfont.ttf", BootDevice, CONFIGPATH);
	sprintf(LanguagePath, "%s%s%s", BootDevice, DEFAULT_APP_PATH, LANGPATH);
	sprintf(UpdatePath, "%s%s", BootDevice, DEFAULT_APP_PATH);
	sprintf(HomebrewAppsPath, "%sapps/", BootDevice);
	sprintf(TempPath, "%sTemp/", UpdatePath);
	sprintf(ScreenshotPath, "%s", UpdatePath);
	sprintf(LinkListPath, "%sURL_List.xml", UpdatePath);
	strcpy(MusicPath, "");
	sprintf(MPlayerPath, "%sapps/mplayer_ce/boot.dol", BootDevice);
	sprintf(WiiMCPath, "%sapps/wiimc/boot.dol", BootDevice);
	LastUsedPath = BootDevice;

	for(int i = 0; i < MAXSMBUSERS; i++) {
		strcpy(SMBUser[i].Host, "");
		strcpy(SMBUser[i].User, "");
		strcpy(SMBUser[i].Password, "");
		strcpy(SMBUser[i].SMBName, "");
	}

	for(int i = 0; i < MAXFTPUSERS; i++) {
		strcpy(FTPUser[i].Host, "");
		strcpy(FTPUser[i].User, "");
		strcpy(FTPUser[i].Password, "");
		strcpy(FTPUser[i].FTPPath, "/");
		FTPUser[i].Port = 21;
		FTPUser[i].Passive = 0;
	}

	FTPServer.AutoStart = 0;
	strcpy(FTPServer.Password, "");
	FTPServer.Port = 21;

	for(int i = 0; i < MAXNFSUSERS; i++) {
		strcpy(NFSUser[i].Host, "");
		strcpy(NFSUser[i].Mountpoint, "");
	}

	FileExtensions.SetDefault();
	Controls.SetDefault();

	DefaultColors();
}

void CSettings::DefaultColors()
{
	BackgroundUL = GXCOLORTORGBA(((GXColor){67, 71, 72, 255}));
	BackgroundUR = GXCOLORTORGBA(((GXColor){67, 71, 72, 255}));
	BackgroundBR = GXCOLORTORGBA(((GXColor){100, 110, 110, 255}));
	BackgroundBL = GXCOLORTORGBA(((GXColor){100, 110, 110, 255}));
	ProgressUL = GXCOLORTORGBA(((GXColor){239, 28, 28, 255}));
	ProgressUR = GXCOLORTORGBA(((GXColor){79, 153, 239, 255}));
	ProgressBR = GXCOLORTORGBA(((GXColor){59, 159, 223, 255}));
	ProgressBL = GXCOLORTORGBA(((GXColor){199, 71, 99, 255}));
	ProgressEmptyUL = GXCOLORTORGBA(((GXColor){237, 240, 245, 255}));
	ProgressEmptyUR = GXCOLORTORGBA(((GXColor){237, 240, 245, 255}));
	ProgressEmptyBR = GXCOLORTORGBA(((GXColor){197, 202, 210, 255}));
	ProgressEmptyBL = GXCOLORTORGBA(((GXColor){197, 202, 210, 255}));
}

bool CSettings::Save()
{
	//if(!FindConfig())
		return false;
}

bool CSettings::FindConfig()
{
	return false;
}

bool CSettings::Load()
{
	return false;

}

bool CSettings::LoadLanguage(const char *path, int language)
{
	bool ret = false;

	if(language >= 0 || !path) {

		if(language < 0)
			return false;

		char filepath[150];
		char langpath[150];
		memset(langpath, 0, sizeof(langpath));
		snprintf(langpath, sizeof(langpath), "%s", LanguagePath);
		//! append slash if it is not a .lang file otherwise get the path of the .lang file
		if(strextcmp(langpath, "lang", '.') == 0)
		{
			char * ptr = strrchr(langpath, '/');
			if(ptr)
			{
				ptr++;
				ptr[0] = '\0';
			}
		}
		else {
			if(strlen(langpath) > 0 && langpath[strlen(langpath)-1] != '/')
				strncat(langpath, "/", sizeof(langpath)-1);
		}

		if(language == APP_DEFAULT)
		{
			strcpy(LanguagePath, langpath);
			gettextCleanUp();
			return true;
		}
		else if(language == CONSOLE_DEFAULT)
		{
			return LoadLanguage(NULL, CONF_GetLanguage()+2);
		}

		else if(language == ENGLISH)
		{
			snprintf(filepath, sizeof(filepath), "%s/english.lang", langpath);
		}


		ret = gettextLoadLanguage(filepath);
		if(ret)
			strncpy(LanguagePath, filepath, sizeof(LanguagePath));

	} else {

		ret = gettextLoadLanguage(path);
		if(ret)
			strncpy(LanguagePath, path, sizeof(LanguagePath));
	}

	return ret;
}

bool CSettings::Reset()
{
	return false;
}

bool CSettings::SetSetting(char *name, char *value)
{
	return false;
}

void CSettings::ParseLine(char *line)
{
}

void CSettings::TrimLine(char *dest, char *src, int size)
{
}
