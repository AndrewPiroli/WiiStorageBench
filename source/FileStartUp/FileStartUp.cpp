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
#include <gctypes.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
//#include "SoundOperations/MusicPlayer.h"
#include "Prompts/PromptWindows.h"
#include "Prompts/ProgressWindow.h"
#include "FileStartUp/FileStartUp.h"
#include "TextOperations/TextEditor.h"
#include "TextOperations/PDFViewer.hpp"
#include "TextOperations/FontSystem.h"
#include "FileOperations/fileops.h"
#include "ImageOperations/ImageConverterGUI.hpp"
#include "ImageOperations/ImageViewer.h"
#include "Controls/Application.h"
#include "VideoOperations/WiiMovie.hpp"
#include "FileExtensions.h"
#include "MPlayerArguements.h"
#include "WiiMCArguemnts.h"
#include "Tools/uncompress.h"

int FileStartUp(const char *filepath)
{
	if(!filepath)
		return -1;

	char *fileext = strrchr(filepath, '.');
	char *filename = strrchr(filepath, '/')+1;
	int choice = WindowPrompt(filename, tr("Do you want to open this file in TextEditor?"), tr("Yes"), tr("No"));
	if(choice){
		TextEditor::LoadFile(filepath);
	}

	return 0;
}
