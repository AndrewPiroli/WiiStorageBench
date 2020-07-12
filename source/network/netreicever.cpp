 /***************************************************************************
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
 * netreceiver.cpp
 *
 * for WiiXplorer 2010
 ***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ogcsys.h>
#include <zlib.h>
#include <fcntl.h>
#include <ogc/machine/processor.h>
//#include "ArchiveOperations/Archive.h"
#include "Prompts/PromptWindows.h"
#include "Prompts/ProgressWindow.h"

#include "http.h"
#include "networkops.h"
#include "FileOperations/fileops.h"
#include "FileOperations/DirList.h"
#include "netreceiver.h"

#define PORT	4299

NetReceiver::NetReceiver()
{
	connection = -1;
	socket = -1;
	filebuffer = NULL;
	filesize = 0;
	uncfilesize = 0;
	memset(FileName, 0, sizeof(FileName));
}

NetReceiver::~NetReceiver()
{
	FreeData();
	CloseConnection();
}

void NetReceiver::CloseConnection()
{
	if(connection >= 0)
		net_close(connection);
	if(socket >= 0)
		net_close(socket);
	connection = -1;
	socket = -1;
}

const u8 * NetReceiver::GetData()
{
	return filebuffer;
}

u32 NetReceiver::GetFilesize()
{
	if(uncfilesize > 0)
		return uncfilesize;

	return filesize;
}

void NetReceiver::FreeData()
{
	if(filebuffer)
		free(filebuffer);
	filebuffer = NULL;
	filesize = 0;
	uncfilesize = 0;
	memset(FileName, 0, sizeof(FileName));
}

bool NetReceiver::CheckIncomming()
{
	return false;
}

const u8 * NetReceiver::ReceiveData()
{
		return NULL;
}

const u8 * NetReceiver::UncompressData()
{
		return NULL;
}
