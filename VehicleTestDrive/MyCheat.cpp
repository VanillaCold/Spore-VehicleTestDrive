#include "stdafx.h"
#include "MyCheat.h"
//#include <Spore/Sporepedia/ObjectTemplateDB.h>
#include "MyGameMode.h"
#include <Spore\Editors\BakeManager.h>

MyCheat::MyCheat()
{
	//sInstance = this;
}


MyCheat::~MyCheat()
{
}


void MyCheat::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	//Editor.mpEditorModel->
	if (!line.HasFlag("loadeditormodel"))
	{
		Sporepedia::ShopperRequest request(this);
		request.shopperID = id("vle_Templateshopper");
		request.Show(request);
	}
	else if (GameModeManager.GetActiveModeID() == kEditorMode)
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
				if (selection.groupID != 0x408A0000) //check if creation is adventure or not
				{
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
				}
				else
				{
					App::ConsolePrintF("This creation cannot be used for Vehicle Test Drive.");
				}
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
			}
		}
	}
}



void MyCheat::OnShopperAccept(const ResourceKey& selection)
{
	if (GameModeManager.GetActiveModeID() == kEditorMode)
	{
		//Editors::EditorRequest::
		SporeDebugPrint(to_string(Editor.mEditorRequest->creationKey.instanceID).c_str());
		SporeDebugPrint(to_string(Editor.mpEditorModel->field_0C.instanceID).c_str());
	}
	if (selection != ResourceKey(0, 0, 0)) //check if creation was even selected or not
	{
		MyGameMode::prevGameMode = GameModeManager.GetActiveModeID();
		//MyGameMode::
		MyGameMode::selection = selection;

		PropertyListPtr propList;
		if (!PropManager.GetPropertyList(selection.instanceID, selection.groupID, propList))
		{
			MyGameMode::editor = new Editors::EditorRequest();
			if (selection.groupID != 0x408A0000) //check if creation is adventure or not
			{
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
			}
			else
			{
				App::ConsolePrintF("This creation cannot be used for Vehicle Test Drive.");
			}
		}
		else
		{
			auto test = Editors::GetEditor();
			test->editorTranslateModelOnSave = test->editorTranslateModelOnSave;
			if (GameModeManager.GetActiveModeID() == kEditorMode) 
			{ 
				MyGameMode::editor = Editor.mEditorRequest; 
				if (Editors::GetEditor()->mpEditorModel != nullptr) { MyGameMode::editor->creationKey = Editors::GetEditor()->mpEditorModel->field_0C; }
			} //if current game mode is editor, give VTD a copy of the current editor request
			else { MyGameMode::editor = nullptr; } //if current game mode isn't the editor, set 'editor' variable for VTD to nullptr
			GameModeManager.SetActiveMode(id("VehicleTestDriveGM"));
		}
	}
}

/*MyCheat* MyCheat::Get()
{
	return sInstance;
}*/

const char* MyCheat::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "MyCheat: Elaborate description of what this cheat does.";
	}
}

//static MyCheat* sInstance;