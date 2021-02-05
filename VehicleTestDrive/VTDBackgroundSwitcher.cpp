#include "stdafx.h"
#include "MyGameMode.h"
#include "VTDBackgroundSwitcher.h"

VTDBackgroundSwitcher::VTDBackgroundSwitcher()
{
}


VTDBackgroundSwitcher::~VTDBackgroundSwitcher()
{
}

// For internal use, do not modify.
int VTDBackgroundSwitcher::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int VTDBackgroundSwitcher::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* VTDBackgroundSwitcher::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(VTDBackgroundSwitcher);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int VTDBackgroundSwitcher::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool VTDBackgroundSwitcher::HandleUIMessage(IWindow* window, const Message& message)
{
	if (message.IsType(kMsgButtonClick))
	{
		PropertyListPtr propList;
		PropManager.GetPropertyList(window->GetCommandID(), GroupIDs::EditorRigblocks, propList);
		float Size = 0;
		float Height = 0;
		float SpaceshipHeight = 0;
		App::Property::GetFloat(propList.get(), id("VTD-BGSize"), Size);
		App::Property::GetFloat(propList.get(), id("VTD-BGHeight"), Height);
		App::Property::GetFloat(propList.get(), id("VTD-BGSpaceshipHeight"), SpaceshipHeight);
		MyGameMode::Get()->SwapBG(window->GetCommandID(),Size,Height,SpaceshipHeight);
	}
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}
