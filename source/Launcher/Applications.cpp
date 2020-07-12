/***************************************************************************
 * Copyright (C) 2010
 * by Dimok, dude
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
 * Application Launcher Class
 *
 * for WiiXplorer 2010
 ***************************************************************************/

//#include <mxml.h>
#include <algorithm>

#include "Applications.h"
#include "FileOperations/DirList.h"
#include "Controls/Clipboard.h"
#include "Controls/Taskbar.h"

Applications::Applications(const char * path)
{
	if(!path)
		return;

	Search(path);

	if(applications.size() > 1)
		Sort();
}

void Applications::Launch(int index)
{
}


bool Applications::GetNameFromXML(const char *xml, char *name)
{
	return false;
}

void Applications::Reload()
{
}

void Applications::Search(const char * path)
{
}

void Applications::Sort()
{
}

bool Applications::SortCallback(const App & f1, const App & f2)
{
		return false;
}
