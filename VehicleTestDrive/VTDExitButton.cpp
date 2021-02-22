#include "stdafx.h"
#include "VTDExitButton.h"
#include "MyGameMode.h"

VTDExitButton::VTDExitButton()
{
}


VTDExitButton::~VTDExitButton()
{
}

// For internal use, do not modify.
int VTDExitButton::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int VTDExitButton::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* VTDExitButton::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(VTDExitButton);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int VTDExitButton::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool VTDExitButton::HandleUIMessage(IWindow* window, const Message& message)
{
	if (message.IsType(kMsgButtonClick))
	{
		if (MyGameMode::prevGameMode != 0 && MyGameMode::prevGameMode != kEditorMode)
		{
			GameModeManager.SetActiveMode(MyGameMode::prevGameMode);
		}
		else { GameModeManager.SetActiveMode(kGGEMode); }
		return true;
	}
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}
