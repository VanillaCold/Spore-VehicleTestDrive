// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "MyCheat.h"
#include "MyGameMode.h"
#include "VTDCamera.h"
#include "VTDEditorButton.h"

App::ICamera* VTDCameraFactory(App::PropertyList* propList)
{
	auto camera = new VTDCamera();
	return camera;
}

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
	//CameraManager.AddCameraType(id("VTDCamera"), &VTDCameraFactory);
	GameModeManager.AddGameMode(new MyGameMode(), id("VehicleTestDriveGM"), "VehicleTestDriveGM");
	CheatManager.AddCheat("entercustomgamemode", new MyCheat());
	CameraManager.PutCamera(id("VTDCamera"), new VTDCamera());
	//auto gm = MyGameMode::Get();
	//gm->world = ModelManager.CreateWorld(id("chocice75modelworld"));
}

void Dispose()
{
	// This method is called when the game is closing
}

member_detour(VTDEditorDetour, Editors::cEditor, bool())
{
	bool detoured()
	{
		auto origfun = original_function(this); //run the original function first, so that the UI will actually exist.
		auto window = WindowManager.GetMainWindow()->FindWindowByID(0x066FF240); //find window that normally obstructs test drive button in non-creature editors
		if (window != nullptr)
		{
			SporeDebugPrint("The detour is working!");
			bool isskin = 0;
			App::Property::GetBool(mpPropList.get(), 0x300DE90B, isskin); //check if editor uses skin.
			if (isskin == 0) //if editor uses skin, just skip all of the following code because skin-based editors work fine for the normal test drive
			{
				auto window2 = window->FindWindowByID(0);
				if (window2 != nullptr) //check if 'Test Drive in All Editors' mod is installed
				{
					window2->AddWinProc(new VTDEditorButton); //if so, just use the button added by that mod instead of making a new button.
					return origfun;
				}
				auto layout = new UTFWin::UILayout;
				layout->LoadByName(u"vtd_button"); //load in vtd_button spui
				layout->SetParentWindow(window); //set parent window for button
				auto window3 = window->FindWindowByID(id("vtd_editorbutton")); //find button through control ID
				window3->SetArea(Math::Rectangle(10, 10, 56, 56)); //set position of button

				if (window3 != nullptr) window3->AddWinProc(new VTDEditorButton); //set win proc for button
			}
		}
		return origfun;
	}
};

void AttachDetours()
{
	VTDEditorDetour::attach(GetAddress(Editors::cEditor, OnEnter));
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

