#include "stdafx.h"
#include "MyGameMode.h"
#include "VTDSporepediaButton.h"
#include <Spore\Editors\BakeManager.h>

VTDSporepediaButton::VTDSporepediaButton()
{
}


VTDSporepediaButton::~VTDSporepediaButton()
{
}

// For internal use, do not modify.
int VTDSporepediaButton::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int VTDSporepediaButton::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* VTDSporepediaButton::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(VTDSporepediaButton);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int VTDSporepediaButton::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool VTDSporepediaButton::HandleUIMessage(IWindow* window, const Message& message)
{
	if (message.IsType(kMsgButtonClick))
	{
		Sporepedia::ShopperRequest request(this);
		request.shopperID = id("vle_Templateshopper");
		//request.
		//request.field_30 = 
		request.Show(request);
		MyGameMode::layout.SetVisible(false);
		return true;
	}
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}

void VTDSporepediaButton::OnShopperAccept(const ResourceKey& selection)
{
	PropertyListPtr propList;
	if (!PropManager.GetPropertyList(selection.instanceID, selection.groupID, propList))
	{
		if (selection != ResourceKey(0, 0, 0))
		{ /*App::ConsolePrintF("Error: The creation is not baked. Preview it in the Sporepedia before loading it into this game mode.");*/
			BakeManager.func4Ch(selection, NULL);
			MyGameMode::selection = selection;
			GameModeManager.SetActiveMode(kGGEMode);
			GameModeManager.SetActiveMode(id("VehicleTestDriveGM"));
		}
		else
		{
			MyGameMode::layout.SetVisible(true);
		}
	}
	else
	{
		if (selection != ResourceKey(0, 0, 0))
		{
			MyGameMode::selection = selection;
			GameModeManager.SetActiveMode(kGGEMode);
			GameModeManager.SetActiveMode(id("VehicleTestDriveGM"));
		}
		else
		{
			MyGameMode::layout.SetVisible(true);
		}
	}
}
