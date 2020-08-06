/****************************************************************************
 * Copyright (C) 2011 Dimok
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
#include <stdio.h>
#include <malloc.h>
#include <sys/dir.h>
#include <algorithm>
#include "MD5Task.h"
#include "Language/gettext.h"
#include "FileOperations/fileops.h"
#include "Prompts/ProgressWindow.h"
#include "DirList.h"

#define BLOCKSIZE   102400

MD5Task::MD5Task(const ItemMarker *p, const std::string &dest)
	: ProcessTask(tr("Calculating MD5"), p, dest),
	  LogFile(NULL), FolderCounter(0), FileCounter(0), ErrorCounter(0)
{
}

MD5Task::~MD5Task(){}

void MD5Task::Execute(void)
{
	TaskBegin(this);
	if(Process.IsItemDir(0)){
		TaskEnd(this);
		return;
	}

	if(Process.GetItemcount() == 0)
	{
		TaskEnd(this);
	}

	if(ProgressWindow::Instance()->IsRunning())
		ProgressWindow::Instance()->SetTitle(tr("Benchmarking"));
	else
		StartProgress(tr("Benchmarking"));

	list<ItemList> itemList;
	GetItemList(itemList, false);
	list<ItemList>().swap(itemList);

	ProgressWindow::Instance()->SetCompleteValues(0, CopySize);

	FolderCounter = 0;
	FileCounter = 0;
	ErrorCounter = 0;

	char currentpath[1024];

	for(int i = 0; i < Process.GetItemcount(); i++)
	{
		if(ProgressWindow::Instance()->IsCanceled())
			break;

		if(!Process.IsItemDir(i))
		{
			snprintf(currentpath, sizeof(currentpath), "%s", Process.GetItemPath(i));
			CalculateFile(currentpath);
		}
	}

	TaskEnd(this);
}

bool MD5Task::CalculateFile(const char * filepath)
{
	if(!filepath)
	{
		++ErrorCounter;
		return false;
	}

	char * filename = strrchr(filepath, '/');
	if(!filename)
	{
		++ErrorCounter;
		return false;
	}

	filename++;

	int read = 0;
	u64 done = 0;
	u64 filesize = FileSize(filepath);
	if (filesize < 1000000){//less than 1MB
		ThrowMsg(tr("Error"), tr("File too small"));
		return false;
	}
	//ThrowMsg(tr("FileSize(filepath)"), tr(fmt("%u", filesize)));
	FILE * file = fopen(filepath, "rb");

	if (file == NULL || filesize == 0)
	{
		++ErrorCounter;
		return false;
	}

	u8 * buffer = (u8 *) malloc(BLOCKSIZE);

	if(!buffer)
	{
		fclose(file);
		++ErrorCounter;
		return false;
	}

	do
	{
		if(ProgressWindow::Instance()->IsCanceled())
		{
			fclose(file);
			free(buffer);
			return false;
		}

		ShowProgress(done, filesize, filename);

		read = fread(buffer, 1, BLOCKSIZE, file);

		done += read;
	}
	while(read > 0);

	fclose(file);
	free(buffer);

	// finish up the progress for this file
	if(ProgressWindow::Instance()->GetWindowOpenStatus()){
		char * tmp = strdup(ProgressWindow::Instance()->GetSpeedTxt()->toUTF8().c_str());
		if(tmp){
			ThrowMsg(tr("Final result:"),tr(tmp));
			free(tmp);
			tmp = NULL;
		}
		else{
			ThrowMsg(tr("Error"), tr("Could not get memory to store result"));
		}
	}
	else{
		ThrowMsg(tr("Error"), tr("File too small"));
	}
	FinishProgress(filesize);

	++FileCounter;

	return true;
}

