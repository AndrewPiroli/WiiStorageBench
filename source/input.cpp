/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * input.cpp
 * Wii/GameCube controller management
 ***************************************************************************/
#include "stdafx.h"
#include "Controls/Application.h"
#include "GUI/gui_trigger.h"
#include "Settings.h"
#include "input.h"

static int rumbleCount[4] = {0,0,0,0};
static int rumbleRequest[4] = {0,0,0,0};
GuiTrigger userInput[4];

/****************************************************************************
 * UpdatePads
 *
 * Scans pad and wpad
 ***************************************************************************/
void UpdatePads()
{
	WPAD_ScanPads();
	PAD_ScanPads();

	for(int i=3; i >= 0; i--)
	{
		memcpy(&userInput[i].wpad, WPAD_Data(i), sizeof(WPADData));

		userInput[i].pad.btns_d = PAD_ButtonsDown(i);
		userInput[i].pad.btns_u = PAD_ButtonsUp(i);
		userInput[i].pad.btns_h = PAD_ButtonsHeld(i);
		userInput[i].pad.stickX = PAD_StickX(i);
		userInput[i].pad.stickY = PAD_StickY(i);
		userInput[i].pad.substickX = PAD_SubStickX(i);
		userInput[i].pad.substickY = PAD_SubStickY(i);
		userInput[i].pad.triggerL = PAD_TriggerL(i);
	}
}

/****************************************************************************
 * Initialize Stuff
 ***************************************************************************/
extern "C" void __Wpad_PowerCallback(s32 chan UNUSED)
{
	Application::shutdownSystem();
}

/****************************************************************************
 * SetupPads
 *
 * Sets up userInput triggers for use
 ***************************************************************************/
void SetupPads()
{
	PAD_Init();
	WPAD_Init();

	WPAD_SetPowerButtonCallback(__Wpad_PowerCallback);

	// read wiimote accelerometer and IR data
	WPAD_SetDataFormat(WPAD_CHAN_ALL,WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_ALL, screenwidth, screenheight);

	for(int i=0; i < 4; i++){
		userInput[i].chan = i;
	}
}

/****************************************************************************
 * Shutdown Pads
 ***************************************************************************/
void ShutdownPads()
{
	ShutoffRumble();
	WPAD_Shutdown();
}

/****************************************************************************
 * Request for Rumble
 ***************************************************************************/
void RequestRumble(int chan){}


/****************************************************************************
 * DoRumble
 ***************************************************************************/
void DoRumble(int i){}
/****************************************************************************
 * ShutoffRumble
 ***************************************************************************/
void ShutoffRumble(){}
