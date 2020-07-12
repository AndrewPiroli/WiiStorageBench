/****************************************************************************
 * Copyright (C) 2009-2011 Dimok
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
#include <time.h>
#include "Taskbar.h"
#include "Controls/Application.h"
#include "Controls/ExternalKeyboard.h"
#include "Memory/Resources.h"
#include "Prompts/PromptWindows.h"
#include "Prompts/ProgressWindow.h"
#include "Prompts/PopUpMenu.h"
#include "DeviceControls/DeviceHandler.hpp"
#include "DeviceControls/PartitionFormatterGUI.hpp"
#include "DeviceControls/RemountTask.h"
//#include "FTPOperations/FTPServerMenu.h"
#include "Launcher/Applications.h"
//#include "SoundOperations/SoundHandler.hpp"
//#include "SoundOperations/MusicPlayer.h"
#include "input.h"
#include "sys.h"

#include "Menus/Explorer.h"
//#include "Menus/Settings/MainSettingsMenu.h"

enum
{
	APPS = 0,
	CHANNELS,
	URLS,
	BOOTMII,
	FORMATTER,
	SETTINGS,
	FTPSERVER,
	REMOUNT,
	RESTART,
	EXIT,
};

Taskbar *Taskbar::instance = NULL;

Taskbar::Taskbar()
	: GuiFrame(0, 0)
{
	WifiData = NULL;
	WifiImg = NULL;

	taskbarImgData = Resources::GetImageData("taskbar.png");
	taskbarImg = new GuiImage(taskbarImgData);

	width = taskbarImg->GetWidth();
	height = taskbarImg->GetHeight();

	timeTxt = new GuiText((char *) NULL, 20, (GXColor) {40, 40, 40, 255});
	timeTxt->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	timeTxt->SetPosition(width-82, -1);
	timeTxt->SetFont(Resources::GetFile("clock.ttf"), Resources::GetFileSize("clock.ttf"));

	soundClick = Resources::GetSound("button_click.wav");
	soundOver = Resources::GetSound("button_over.wav");
	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	startBtn = new PictureButton("start.png", "start_over.png", soundClick, soundOver);
	startBtn->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	startBtn->SetPosition(23, -2);
	startBtn->SetSelectable(false);
	startBtn->SetTrigger(trigA);
	startBtn->Clicked.connect(this, &Taskbar::OnStartButtonClick);

	HeadPhonesData = Resources::GetImageData("player_icon.png");
	HeadPhonesImg = new GuiImage(HeadPhonesData);
	//Musicplayer = new GuiButton(HeadPhonesData->GetWidth(), HeadPhonesData->GetHeight());
	//Musicplayer->SetImage(HeadPhonesImg);
	//Musicplayer->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	//Musicplayer->SetTrigger(trigA);
	//Musicplayer->SetPosition(458, 0);
	//Musicplayer->SetEffectGrow();
	//Musicplayer->Clicked.connect(this, &Taskbar::OnMusicPlayerClick);

	Append(taskbarImg);
	//Append(startBtn);
	//Append(Musicplayer);
	Append(timeTxt);

	SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
	SetPosition(0, -15);

	//! Open first explorer
	mainExplorer = new Explorer(Application::Instance(), Settings.LastUsedPath.c_str());
}

Taskbar::~Taskbar()
{
	RemoveAll();

	Resources::Remove(taskbarImgData);
	Resources::Remove(HeadPhonesData);
	Resources::Remove(WifiData);

	delete taskbarImg;
	delete HeadPhonesImg;
	delete WifiImg;

	delete startBtn;
	delete Musicplayer;
	delete timeTxt;

	delete trigA;

	delete mainExplorer;

	Resources::Remove(soundClick);
	Resources::Remove(soundOver);
}

void Taskbar::AddTask(Task * t)
{
	t->SetPosition(95+Tasks.size()*100, 0);
	t->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	t->TaskEnd.connect(this, &Taskbar::RemoveTask);
	Tasks.push_back(t);
	Append(t);
}

void Taskbar::RemoveTask(Task * t)
{
	for(u32 i = 0; i < Tasks.size(); i++)
	{
		//! move all tasks in task bar to the left if needed
		Tasks[i]->SetPosition(95+i*100, 0);

		if(Tasks[i] == t)
		{
			Remove(t);
			Tasks.erase(Tasks.begin()+i);
			i--;
		}
	}
}

void Taskbar::Draw()
{
	if(frameCount % 60 == 0) //! Update time value every sec
	{
		char timetxt[20];
		time_t currenttime = time(0);
		struct tm * timeinfo = localtime(&currenttime);

		if(Settings.ClockMode == 0)
			strftime(timetxt, sizeof(timetxt), "%H:%M:%S", timeinfo);
		else
			strftime(timetxt, sizeof(timetxt), "%I:%M:%S", timeinfo);

		timeTxt->SetText(timetxt);
	}

	

	GuiFrame::Draw();
}

void Taskbar::OnStartButtonClick(GuiButton *sender, int pointer, const POINT &p UNUSED)
{

}

void Taskbar::OnStartmenuItemClick(PopUpMenu *menu, int item)
{
}

void Taskbar::OnAppsMenuClick(PopUpMenu *menu, int item)
{

}

void Taskbar::OnChannelsMenuClick(PopUpMenu *menu, int item)
{

}

void Taskbar::OnUrlsMenuClick(PopUpMenu *menu, int item)
{
}

void Taskbar::OnMusicPlayerClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p3 UNUSED)
{
}

void Taskbar::OnMenuClosing(GuiFrame *menu UNUSED)
{
	//! show main explorer
	mainExplorer->show();
}
