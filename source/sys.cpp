 /****************************************************************************
 * Copyright (C) 2010
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 * for WiiXplorer 2010
 ***************************************************************************/
#include "Prompts/PromptWindows.h"
#include "Prompts/ProgressWindow.h"
#include "Controls/Application.h"
#include "Controls/Clipboard.h"
#include "Controls/Taskbar.h"
#include "DeviceControls/DeviceHandler.hpp"
#include "Memory/mem2.h"
#include "VideoOperations/video.h"
#include "TextOperations/FontSystem.h"
#include "FileOperations/fileops.h"
#include "DiskOperations/di2.h"
#include "Tools/tools.h"
#include "audio.h"
#include "input.h"
#include "sys.h"
#include <sys/param.h>

extern "C" bool RebootApp()
{
		return false;
}

extern "C" void ExitApp()
{
	//! this should never happen, but its just in case here
	static bool bRunOnce = false;
	if(bRunOnce)
		return;
	bRunOnce = true;

	if(Settings.DeleteTempPath)
		RemoveDirectory(Settings.TempPath);

	Settings.Save();
	//! fade out
	//! now destroy objects
	Application::Instance()->quit();
	Clipboard::DestroyInstance();
	Taskbar::DestroyInstance();
	ProgressWindow::DestroyInstance();
	Application::DestroyInstance();
	Resources::DestroyInstance();
	DeviceHandler::DestroyInstance();
	StopGX();
	ShutdownAudio();
	ClearFontData();
	DI2_Close();
	USB_Deinitialize();
	ShutdownPads();
	ISFS_Deinitialize();
	MagicPatches(0);
}

extern "C" void __Sys_ResetCallback(void)
{
	Application::resetSystem();
}

extern "C" void __Sys_PowerCallback(void)
{
	Application::shutdownSystem();
}


extern "C" void Sys_Init(void)
{
	SYS_SetResetCallback((resetcallback)__Sys_ResetCallback);
	SYS_SetPowerCallback(__Sys_PowerCallback);
}

extern "C" void Sys_Reboot(void)
{
	ExitApp();
	STM_RebootSystem();
}

#define ShutdownToDefault	0
#define ShutdownToIdle		1
#define ShutdownToStandby	2

static void _Sys_Shutdown(int SHUTDOWN_MODE)
{
	ExitApp();

	STM_ShutdownToStandby();	
}

extern "C" void Sys_Shutdown(void)
{
	_Sys_Shutdown(ShutdownToDefault);
}

extern "C" void Sys_ShutdownToIdle(void)
{
	_Sys_Shutdown(ShutdownToIdle);
}

extern "C" void Sys_ShutdownToStandby(void)
{
	_Sys_Shutdown(ShutdownToStandby);
}

extern "C" void Sys_LoadMenu(void)
{
	ExitApp();

	/* Return to the Wii system menu */
	SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

extern "C" void Sys_BackToLoader(void)
{
	ExitApp();

	if (IsFromHBC())
		Sys_LoadHBC();

	// Channel Version
	SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

extern "C" bool IsFromHBC()
{

		return true;
}

#define HBC_HAXX	0x0001000148415858LL
#define HBC_JODI	0x000100014A4F4449LL
#define HBC_1_0_7	0x00010001AF1BF516LL
#define HBC_LULZ	0x000100014c554c5aLL

extern "C" void Sys_LoadHBC(void)
{
	ExitApp();
	SYS_ResetSystem(SYS_SHUTDOWN, 0,0 );
	exit(0);
	WII_Initialize();

	int ret = WII_LaunchTitle(HBC_LULZ);
	if(ret < 0)
		WII_LaunchTitle(HBC_1_0_7);
	if(ret < 0)
		WII_LaunchTitle(HBC_JODI);
	if(ret < 0)
		WII_LaunchTitle(HBC_HAXX);

	//Back to system menu if all fails
	SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

extern "C" int GetIOS_Rev(u32 ios)
{
	u32 num_titles = 0, i = 0;
	u64 tid = 0;
	u64 * titles = NULL;
	s32 ret = -1;

	ret = ES_GetNumTitles(&num_titles);
	if(ret < 0)
		return -1;

	if(num_titles < 1)
		return -1;

	titles = (u64 *) memalign(32, ALIGN32(num_titles * sizeof(u64) + 32));
	if(!titles)
		return -1;

	ret = ES_GetTitles(titles, num_titles);
	if(ret < 0)
	{
		free(titles);
		return -1;
	}

	for(i=0; i < num_titles; i++)
	{
		if ((titles[i] & 0xFFFFFFFF) == ios)
		{
			tid = titles[i];
			break;
		}
	}

	free(titles);
	titles = NULL;

	if(!tid)
		return -1;

	ISFS_Initialize();

	char tmd[ISFS_MAXPATH];
	static fstats stats ATTRIBUTE_ALIGN(32);
	ret = -1;

	u32 high = (u32)(tid >> 32);
	u32 low  = (u32)(tid & 0xFFFFFFFF);

	sprintf(tmd, "/title/%08x/%08x/content/title.tmd", high, low);

	s32 fd = ISFS_Open(tmd, ISFS_OPEN_READ);
	if (fd >= 0)
	{
		if (ISFS_GetFileStats(fd, &stats) >= 0)
		{
			u32 * data = NULL;

			if (stats.file_length > 0)
				data = (u32 *) memalign(32, ALIGN32(stats.file_length));

			if (data)
			{
				if (ISFS_Read(fd, (char *) data, stats.file_length) > 0x208)
				{
					ret = ((struct _tmd *) SIGNATURE_PAYLOAD(data))->title_version;
				}
				free(data);
			}
		}
		ISFS_Close(fd);
	}

	ISFS_Deinitialize();

	return ret;
}

extern "C" bool FindTitle(u64 titleid)
{
	bool found = false;
	u32 num_titles = 0, i = 0;
	u64 * titles = NULL;
	s32 ret = 0;

	ret = ES_GetNumTitles(&num_titles);
	if(ret < 0)
		return found;

	if(num_titles < 1)
		return found;

	titles = (u64 *) memalign(32, ALIGN32(num_titles * sizeof(u64) + 32));
	if(!titles)
		return found;

	ret = ES_GetTitles(titles, num_titles);
	if(ret < 0)
	{
		free(titles);
		return found;
	}

	for(i=0; i < num_titles; i++)
	{
		if (titles[i] == titleid)
		{
			found = true;
			break;
		}
	}

	free(titles);

	return found;
}
