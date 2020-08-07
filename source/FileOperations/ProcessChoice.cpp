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
#include "Menus/Explorer.h"
#include "ProcessChoice.h"
#include "Controls/Clipboard.h"
#include "Controls/Application.h"
#include "Controls/Taskbar.h"
#include "Controls/ThreadedTaskHandler.hpp"
#include "Prompts/Properties.h"
#include "Prompts/ArchiveProperties.h"
#include "Prompts/ProgressWindow.h"
#include "Prompts/PromptWindows.h"
#include "Prompts/PopUpMenu.h"
#include "FileOperations/fileops.h"
#include "FileOperations/MD5Task.h"

void Explorer::ProcessArcChoice(int choice, const char * destCandidat)
{
	return;

}

void Explorer::ProcessChoice(int choice)
{
	FileBrowser * browser = (FileBrowser *) curBrowser;
	if(!browser)
		return;
	
	if(choice == CHECK_MD5)
	{
		int md5Choice = 1;

		if(ProgressWindow::Instance()->IsRunning())
			md5Choice = WindowPrompt(tr("Currently a process is running."), tr("Do you want to append this process to the queue?"), tr("Yes"), tr("Cancel"));

		if(md5Choice == 1)
		{
			char LogPath[1024];
			snprintf(LogPath, sizeof(LogPath), "%s/MD5.log", browser->GetCurrentPath());

			browser->MarkCurrentItem();
			MD5Task *task = new MD5Task(browser->GetItemMarker(), LogPath);
			task->TaskEnd.connect(this, &Explorer::OnFinishedTask);
			this->explorerTasks++;
			Taskbar::Instance()->AddTask(task);
			ThreadedTaskHandler::Instance()->AddTask(task);
			browser->GetItemMarker()->Reset();
		}
	}
	
	if(choice == PROPERTIES)
	{
		browser->MarkCurrentItem();
		ItemMarker * Marker = browser->GetItemMarker();
		Properties * Prompt = new Properties(Marker);
		Prompt->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
		Prompt->DimBackground(true);
		Application::Instance()->SetUpdateOnly(Prompt);
		Application::Instance()->Append(Prompt);
		Marker->Reset();
	}
}
