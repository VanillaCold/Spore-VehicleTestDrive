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
	Sporepedia::ShopperRequest request(this);
	request.shopperID = id("vle_Templateshopper");
	request.Show(request);
}



void MyCheat::OnShopperAccept(const ResourceKey& selection)
{
	MyGameMode::prevGameMode = GameModeManager.GetActiveModeID();
	MyGameMode::selection = selection;
	//cAssetMetadataPtr intptrthingy;
	//Pollinator::GetMetadata(selection.instanceID, selection.groupID, intptrthingy);
	//intptrthingy->
	
	PropertyListPtr propList;
	if (!PropManager.GetPropertyList(selection.instanceID, selection.groupID, propList))
	{
		//cAssetMetadataPtr metadata;
		//Pollinator::GetMetadata(selection.instanceID,selection.groupID,metadata);
		//App::ConsolePrintF("Error: The creation is not baked. Preview it in the Sporepedia before loading it into this game mode.");
		if (selection.groupID != 0x408A0000)
		{
			BakeManager.func4Ch(selection, NULL);
			GameModeManager.SetActiveMode(id("VehicleTestDriveGM"));
		}
		else 
		{
			App::ConsolePrintF("This creation cannot be used for Vehicle Test Drive.");
		}
	}
	else
	{
		GameModeManager.SetActiveMode(id("VehicleTestDriveGM"));
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