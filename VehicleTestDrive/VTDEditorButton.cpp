#include "stdafx.h"
#include "VTDEditorButton.h"
#include "MyGameMode.h"
#include <Spore/Editors/BakeManager.h>

VTDEditorButton::VTDEditorButton()
{
}


VTDEditorButton::~VTDEditorButton()
{
}

// For internal use, do not modify.
int VTDEditorButton::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int VTDEditorButton::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* VTDEditorButton::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(VTDEditorButton);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int VTDEditorButton::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool VTDEditorButton::HandleUIMessage(IWindow* window, const Message& message)
{
	if (auto savebutton = WindowManager.GetMainWindow()->FindWindowByID(0x05B6E484)) {
		if (savebutton->IsEnabled()) { window->SetShadeColor(Math::Color(128, 128, 128, 255));  return false; }
		else { window->SetShadeColor(Math::Color(255, 255, 255, 255)); }
	}
	else { window->SetShadeColor(Math::Color(255, 255, 255, 255)); }
	if (message.IsType(MessageType::kMsgButtonClick) || message.IsType(MessageType::kMsgButtonSelect))
	{
		auto selection = Editor.mpEditorModel->field_0C;
		if (selection != ResourceKey(0, 0, 0)) //check if creation was even selected or not
		{
			MyGameMode::prevGameMode = GameModeManager.GetActiveModeID();
			//MyGameMode::
			MyGameMode::selection = selection;
			PropertyListPtr propList;
			if (!PropManager.GetPropertyList(selection.instanceID, selection.groupID, propList))
			{
				MyGameMode::editor = new Editors::EditorRequest();
				auto test = Editors::GetEditor();
				test->editorTranslateModelOnSave = test->editorTranslateModelOnSave;
				if (GameModeManager.GetActiveModeID() == kEditorMode)
				{
					MyGameMode::editor = Editor.mEditorRequest;
					if (Editors::GetEditor()->mpEditorModel != nullptr) { MyGameMode::editor->creationKey = Editors::GetEditor()->mpEditorModel->field_0C; }
				}
				else { MyGameMode::editor = nullptr; }
				BakeManager.func4Ch(selection, NULL); //bake model
				GameModeManager.SetActiveMode(id("VehicleTestDriveGM")); //set game mode
				return true;
			}
			else
			{
				auto test = Editors::GetEditor();
				//test->editorTranslateModelOnSave = test->editorTranslateModelOnSave;
				if (GameModeManager.GetActiveModeID() == kEditorMode)
				{
					MyGameMode::editor = Editor.mEditorRequest;
					if (Editors::GetEditor()->mpEditorModel != nullptr) { MyGameMode::editor->creationKey = Editors::GetEditor()->mpEditorModel->field_0C; }
				}
				else { MyGameMode::editor = nullptr; }
				//BakeManager.func4Ch(selection, NULL); //bake model
				GameModeManager.SetActiveMode(id("VehicleTestDriveGM")); //set game mode
				return true;
			}
		}
	}
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}
