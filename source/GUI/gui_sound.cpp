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
#include "gui_sound.h"
//#include "SoundOperations/MusicPlayer.h"
//#include "SoundOperations/SoundHandler.hpp"
//#include "SoundOperations/WavDecoder.hpp"
#include "FileOperations/fileops.h"
#include "Tools/uncompress.h"

#define MAX_SND_VOICES	  16

static bool VoiceUsed[MAX_SND_VOICES] =
{
	true, false, false, false, false, false,
	false, false, false, false, false, false,
	false, false, false, false
};

static inline int GetFirstUnusedVoice()
{
	return -1;
}

extern "C" void SoundCallback(s32 voice)
{
		return;
}

GuiSound::GuiSound(const char * filepath)
{
}

GuiSound::GuiSound(const u8 * snd, s32 len, bool isallocated, int v)
{
}

GuiSound::~GuiSound()
{
}

void GuiSound::FreeMemory()
{
}

bool GuiSound::Load(const char * filepath)
{
	return false;

}

bool GuiSound::Load(const u8 * snd, s32 len, bool isallocated)
{

	return false;
}

bool GuiSound::LoadSoundEffect(const u8 * snd, s32 len)
{
	return false;
}

void GuiSound::Play()
{
}
void GuiSound::Stop()
{

}

void GuiSound::Pause()
{

}

void GuiSound::Resume()
{
}

bool GuiSound::IsPlaying()
{
	return false;
}

void GuiSound::SetVolume(int vol)
{
}

void GuiSound::SetLoop(u8 l)
{
}

void GuiSound::Rewind()
{

}

void GuiSound::UncompressSoundbin(const u8 * snd, int len, bool isallocated)
{
}
