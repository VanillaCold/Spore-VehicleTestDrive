#include "stdafx.h"
#include "MyGameMode.h"
#include <Spore\UTFWin\cSPUIMessageBox.h>
#include "VTDSporepediaButton.h"
#include "VTDBackgroundSwitcher.h"
#include <Spore\Sporepedia\AssetViewManager.h>
#include "VTDTestDummy.h"
//#include <Spore\Sporepedia\ObjectTemplateDB.h>
MyGameMode::MyGameMode()
{
}

MyGameMode::~MyGameMode()
{
}

// For internal use, do not touch this.
int MyGameMode::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not touch this.
int MyGameMode::Release()
{
	return DefaultRefCounted::Release();
}

// The use of this function is unknown.
bool MyGameMode::func0Ch()
{
	return false;
}

bool MyGameMode::Initialize(App::IGameModeManager* pManager)
{
	// Called when on game startup. Here you should create all your model/effect worlds, etc.
	return true;
}

bool MyGameMode::Dispose()
{
	// Called when the game exits. Here you should dispose all your model/effect worlds,
	// ensure all objects are released, etc
	return true;
}

bool MyGameMode::OnEnter()
{
	sInstance = this;
	bounds = 65;
	if (spaceship == 0) { spaceship = 0x06576dbd; bounds = 65; }
	chocicetdeffectworld = SwarmManager.CreateWorld(id("VehicleTestDrive"));
	chocicetdeffectworld->SetState(Swarm::kStateActive);
	world = ModelManager.CreateWorld(id("chocice75modelworld"));
	auto swarmworld = SwarmManager.GetActiveWorld();
	SwarmManager.SetActiveWorld(chocicetdeffectworld.get());
	swarmworld = SwarmManager.GetActiveWorld();
	world->SetVisible(true);
	RenderManager.AddRenderable(world->ToRenderable(), Graphics::kRenderQueueMain);



	//AssetViewManager.
	//RenderManager.AddRenderable(world->ToRenderable(), Graphics::kRenderQueueMain);
	// Called when the game mode is entered. Here you should load your effects and models,
	// load the UI, add custom renderers, etc.

	// Return false if there was any error.
	if (model = world->LoadModel(selection.instanceID, selection.groupID))
	{
		//model->mCollisionMode = Graphics::CollisionMode::BoundingBox;
		world->SetFixedLod(model.get(), 0);
		model->SetTransform(model->GetTransform().Multiply(Transform().SetScale(0.75)));
		model->mFlags |= Graphics::kModelFlagHighRes;
		world->UpdateModel(model.get());
	}
	else { return false; }



	uint32_t bg = 0;
	isspaceship = false;
	if (isspaceship == true) { friction = 1.0625; }
	bg = id("BE_editor_base_low");
	
	PropertyListPtr propList;

	if (selection.typeID == TypeIDs::ufo) { isspaceship = true; bg = id("ve_testdrive_space"); }
	else 
	{ 
		bg = id("vehicle_testdrive_bg"); isspaceship = false;
		if (editor != nullptr) 
		{
			if (editor->editorID == id("VehicleColonyAir") || editor->editorID == id("VehicleCulturalAir") || editor->editorID == id("VehicleEconomicAir") || editor->editorID == id("VehicleMilitaryAir"))
			{
				bg = id("ve_testdrive_air");
			}
			
			if (editor->editorID == id("VehicleColonyWater") || editor->editorID == id("VehicleCulturalWater") || editor->editorID == id("VehicleEconomicWater") || editor->editorID == id("VehicleMilitaryWater"))
			{
				bg = id("ve_testdrive_water");
			}
		}
		
	}

	if (PropManager.GetPropertyList(selection.instanceID, selection.groupID, propList) && isspaceship == 0)
	{
		App::Property::GetFloat(propList.get(), 0x720BE500, VehicleSpeed);
		VehicleSpeed = min(max(0.1F, VehicleSpeed), 1.0F) / 1.125;
	}
	else { VehicleSpeed = 1; }
	
	dummy1 = new VTDTestDummy();
	dummy1->Load(selection, Vector3( 15.0F,15.0F,0 ), { 0,0,0 }, 100);

	if (background = world->LoadModel(bg, GroupIDs::EditorRigblocks))
	{
		world->SetFixedLod(background.get(), 0);
		background->SetTransform(background->GetTransform().SetScale(15));
		PropertyListPtr propList;
		PropManager.GetPropertyList(bg, GroupIDs::EditorRigblocks, propList);
		float BGSize = 0;
		float BGHeight = 0;
		float BGSpaceshipHeight = 0;
		App::Property::GetFloat(propList.get(), id("VTD-BGSize"), BGSize);
		App::Property::GetFloat(propList.get(), id("VTD-BGHeight"), BGHeight);
		App::Property::GetFloat(propList.get(), id("VTD-BGSpaceshipHeight"), BGSpaceshipHeight);
		world->UpdateModel(background.get());
		SwapBG(bg, BGSize, BGHeight, BGSpaceshipHeight);



		//if (isspaceship == false && bg == id("vehicle_testdrive_bg")) { background->SetTransform(background->GetTransform().SetOffset(0, 0, 47.4375).SetScale(7.5)); }
		
	}
	else { return false; }

	if (background1 = world->LoadModel(id("ve_testdrive_water"), GroupIDs::EditorRigblocks))
	{
		world->SetFixedLod(background1.get(), 0);
		background1->SetTransform(background1->GetTransform().Multiply(Transform().SetScale(2.5)));
		if (isspaceship == true) { background1->SetTransform(background1->GetTransform().Multiply(Transform().SetOffset(0, 0, -47.4375))); }
		else { background1->SetTransform(background1->GetTransform().Multiply(Transform().SetOffset(0, 0, 0.25))); }
		world->SetModelVisible(background1.get(), false);
		world->UpdateModel(background1.get());
	}
	else { return false; }

	if (background2 = world->LoadModel(id("ve_editor_airbase_background"), GroupIDs::EditorRigblocks))
	{
		world->SetFixedLod(background2.get(), 0);
		background2->SetTransform(background2->GetTransform().SetScale(5));
		world->SetModelVisible(background2.get(), false);
		world->UpdateModel(background2.get());
	}

	else { return false; }
	lightworld = LightingManager.CreateWorld(id("chocice75lightingworldvehicletestdrive"), "chocice75lightingworldvehicletestdrive");
	lightworld->SetConfiguration(id("CreatureEditor"));
	world->AddLightingWorld(lightworld, 0, true);
	targetoffset = { 0,0,0 };
	currentbackground = 0;

	CameraManager.SetActiveCameraByID(id("VTDCamera")); //VehicleTestDriveCam
	cam = VTDCamera::Get();
	window = new UTFWin::Window(); 
//	window->AddWinProc(new UTFWin::ProportionalLayout(0, 0.5, 0, 1));
//	window->SetArea({ -100, -50, 100, 50 });
	WindowManager.GetMainWindow()->AddWindow(window);
	window->FitParentArea(window);
	window->SetFlag(UTFWin::WindowFlags::kWinFlagIgnoreMouse, true);
	layout.LoadByName(u"vtd_spui");
	layout.SetParentWindow(window);
	layout.GetContainerWindow()->FitParentArea(layout.GetContainerWindow());
	auto sporepediabutton = layout.FindWindowByID(0x06FCB630);
	sporepediabutton->AddWinProc(new VTDSporepediaButton());
	if (editor != nullptr) { window->RemoveWindow(sporepediabutton); }
	auto exitbutton = layout.FindWindowByID(0x90439D7C);
	exitbutton->AddWinProc(new VTDExitButton());
	auto thingy1 = layout.FindWindowByID(id("uibutton1"));
	thingy1->AddWinProc(new VTDBackgroundSwitcher());
	auto thingy2 = layout.FindWindowByID(id("uibutton2"));
	if (thingy2 != nullptr) { thingy2->AddWinProc(new VTDBackgroundSwitcher()); }
	auto thingy3 = layout.FindWindowByID(id("uibutton3"));
	if (thingy3 != nullptr) { thingy3->AddWinProc(new VTDBackgroundSwitcher()); }
	auto thingy4 = layout.FindWindowByID(id("uibutton4"));
	if (thingy4 != nullptr) { thingy4->AddWinProc(new VTDBackgroundSwitcher()); }
	auto thingy5 = layout.FindWindowByID(id("uibutton5"));
	if (thingy5 != nullptr) { thingy5->AddWinProc(new VTDBackgroundSwitcher()); }
	auto thingy6 = layout.FindWindowByID(id("uibutton6"));
	if (thingy6 != nullptr) { thingy6->AddWinProc(new VTDBackgroundSwitcher()); }
	auto thingy7 = layout.FindWindowByID(id("uibutton7"));
	if (thingy7 != nullptr) { thingy7->AddWinProc(new VTDBackgroundSwitcher()); }
	auto thingy8 = layout.FindWindowByID(id("uibutton8"));
	if (thingy8 != nullptr) { thingy8->AddWinProc(new VTDBackgroundSwitcher()); }
	return true;
}

void MyGameMode::OnExit()
{
	world->Dispose(); //remove model world
	WindowManager.GetMainWindow()->RemoveWindow(window); //remove VTD SPUI

	//if (GameModeManager.GetActiveModeID() == kEditorMode)
	{
		int i = 0;
		if (prevGameMode == kEditorMode && editor != nullptr && this != nullptr) {
			while (Editors::GetEditor()->mEditorRequest->activeModeID = id("VehicleTestDriveGM")) //make sure that exiting the editor doesn't force you back into VTD after leaving VTD.
			{
				Editors::GetEditor()->mEditorRequest->activeModeID = editor->activeModeID;
				if (editor->activeModeID == id("VehicleTestDriveGM")) 
				{ 
					if (prevEditorGameMode == id("VehicleTestDriveGM"))
					{
						editor->activeModeID = kGGEMode;
					}
					else { editor->activeModeID = prevEditorGameMode; }
				}
				i++;
				if (i > 5000) { break; } //failsafe! Woo!
			}
		}
	}

	// Called when the game mode is exited. Here you should kill all effects and models, 
	// stop any renderers, unload the UI, etc.
	//auto thin = layout.GetContainerWindow();
	//thin->RemoveWindow(thin);
}

// The use of this function is unknown.
void* MyGameMode::func20h(int) 
{
	return nullptr;
}


//// INPUT LISTENERS ////

// Called when a keyboard key button is pressed.
bool MyGameMode::OnKeyDown(int virtualKey, KeyModifiers modifiers)
{
	mInput.OnKeyDown(virtualKey, modifiers);
	/*if (mInput.IsKeyDown(VK_DOWN))
	{
		if (currentbackground == 0)
		{
			currentbackground = 1;
			world->SetModelVisible(background.get(), false);
			world->SetModelVisible(background1.get(), true);
			world->SetModelVisible(background2.get(), false);
			world->UpdateModel(background1.get());
			return true;
		}
		if (currentbackground == 1)
		{
			currentbackground = 2;
			world->SetModelVisible(background.get(), false);
			world->SetModelVisible(background1.get(), false);
			world->SetModelVisible(background2.get(), true);
			world->UpdateModel(background2.get());
			return true;
		}
		if (currentbackground == 2)
		{
			currentbackground = 0;
			world->SetModelVisible(background.get(), true);
			world->SetModelVisible(background1.get(), false);
			world->SetModelVisible(background2.get(), false);
			world->UpdateModel(background.get());
			return true;
		}
	}*/
	// Return true if the keyboard event has been handled in this method.
	return false;
}

// Called when a keyboard key button is released.
bool MyGameMode::OnKeyUp(int virtualKey, KeyModifiers modifiers)
{
	mInput.OnKeyUp(virtualKey, modifiers);
	
	// Return true if the keyboard event has been handled in this method.
	return false;
}

// Called when a mouse button is pressed (this includes the mouse wheel button).
bool MyGameMode::OnMouseDown(MouseButton mouseButton, float mouseX, float mouseY, MouseState mouseState)
{
	mInput.OnMouseDown(mouseButton, mouseX, mouseY, mouseState);
	
	// Return true if the mouse event has been handled in this method.
	return false;
}

// Called when a mouse button is released (this includes the mouse wheel button).
bool MyGameMode::OnMouseUp(MouseButton mouseButton, float mouseX, float mouseY, MouseState mouseState)
{
	mInput.OnMouseUp(mouseButton, mouseX, mouseY, mouseState);
	
	// Return true if the mouse event has been handled in this method.
	return false;
}

// Called when the mouse is moved.
bool MyGameMode::OnMouseMove(float mouseX, float mouseY, MouseState mouseState)
{
	mInput.OnMouseMove(mouseX, mouseY, mouseState);
//	if (mInput.IsMouseButtonDown(kMouseButtonLeft))
//	{
//		targetoffset = { mouseX,mouseY,0 };
//		return true;
//	}

	// Return true if the mouse event has been handled in this method.
	return false;
}

// Called when the mouse wheel is scrolled. 
// This method is not called when the mouse wheel is pressed.
bool MyGameMode::OnMouseWheel(int wheelDelta, float mouseX, float mouseY, MouseState mouseState)
{
	mInput.OnMouseWheel(wheelDelta, mouseX, mouseY, mouseState);
	
	// Return true if the mouse event has been handled in this method.
	return false;
}


//// UPDATE FUNCTION ////

void MyGameMode::Update(float delta1, float delta2)
{
	auto offset = model->GetTransform().GetOffset();
	auto transform = model->GetTransform();

	oldtransform.SetOffset(transform.GetOffset());
	oldtransform.SetRotation(transform.GetRotation());
	oldtransform.SetScale(transform.GetScale());

	//offset = model->GetTransform().GetOffset();
	//transform.SetOffset(min(30.0F, offset.x), min(30.0F, offset.y), min(30.0F, offset.z));
	//offset = transform.GetOffset();
	//transform.SetOffset(max(-30.0F, offset.x), max(-30.0F, offset.y), max(-30.0F, offset.z));
	//offset = model->GetTransform().GetOffset();
	//model->SetTransform(transform);
	//BoundingBox bounds = { { -15.0F, -15.0F, -15.0F}, {15.0F, 15.0F, 15.0F} };
	if (mInput.IsKeyDown(VK_LEFT))
	{
		auto dir = targetdirection.ToEuler();
		targetdirection = Math::Matrix3::FromEuler({ dir.x,dir.y,dir.z + 0.0234375F });
	}

	if (mInput.IsKeyDown(VK_RIGHT))
	{
		auto dir = targetdirection.ToEuler();
		targetdirection = Math::Matrix3::FromEuler({ dir.x,dir.y,dir.z - 0.0234375F });
	}

	if (mInput.IsKeyDown(VK_UP))
	{
		targetoffset = targetdirection * Vector3({ 0,targetoffset.y + 0.03125F,0 });
		if (targetoffset.y > VehicleSpeed) { targetoffset.y = VehicleSpeed; }
		/*auto test = model->GetTransform().Multiply(Transform().SetOffset(targetoffset).SetRotation(targetdirection));
		auto test2 = model->GetTransform().GetOffset();
		if (test.GetOffset().y > 30 || test.GetOffset().x > 30 || test.GetOffset().y < -30 || test.GetOffset().x < -30) { targetoffset = Vector3(test2.x - test.GetOffset().x, test2.y - test.GetOffset().y, 0); }
		else { if (targetoffset.y < 0) { targetoffset.y = 0; } }*/
		auto test = (model->GetTransform().GetOffset() + targetoffset.y);
		//if (test.x  > 15 || test.y > 15 || test.z > 15) { targetoffset.y = 0; }
		//if (test.x < -15 || test.y < -15 || test.z < -15) { targetoffset.y = 0; }
		//if (!bounds.Contains(test)) { model->GetTransform().SetOffset(max(min(15.0F, model->GetTransform().GetOffset().x), -15.0F), max(min(15.0F, model->GetTransform().GetOffset().y), -15.0F), max(min(15.0F, model->GetTransform().GetOffset().z), -15.0F)); }
	}

	offset = model->GetTransform().GetOffset();
	auto direction = model->GetTransform().GetRotation();
	//auto thingth::Matrix3::
	model->SetTransform(model->GetTransform().Multiply(Transform().SetOffset(targetoffset).SetRotation(targetdirection)));
	if ((!mInput.IsKeyDown(VK_UP)) && (isspaceship == true))
	{
		targetoffset /= 1.0625;
	}
	else {
		if ((!mInput.IsKeyDown(VK_UP)) && (isspaceship == false))
		{
			{targetoffset /= 1.125; }
		}
	}
	if (targetoffset.y < 0.015125) { targetoffset.y = 0; }
	if (targetoffset.x < 0.015125) { targetoffset.x = 0; }
	auto dir = targetdirection.ToEuler() /= 1.5;


	if ((targetoffset.x > 0 || targetoffset.y > 0))
	{
		if (!mClock.IsRunning())
		{
			mClock.Start();
		}

		float time = mClock.GetElapsed();

		auto idcount = world->GetAnimCount(model.get());

		uint32_t* ids = new uint32_t [idcount+1];

		world->GetAnimIDs(model.get(), ids);

		float dsStart, dsEnd;

		world->SetAnimTime(model.get(), ids[0], time);
		world->GetAnimRange(model.get(), ids[0], dsStart, dsEnd);

		if (time >= (dsEnd - dsStart)) {
			mClock.Reset();
			mClock.Start();
		}
	}
	else
	{
		mClock.Reset();
	}
	
	/*transform = model->GetTransform();
	offset = model->GetTransform().GetOffset();
	targetdirection = Math::Matrix3::FromEuler(dir);
	transform.SetOffset(min(30.0F, offset.x), min(50.0F, offset.y), min(50.0F, offset.z));
	offset = transform.GetOffset();
	transform.SetOffset(max(-30.0F, offset.x), max(-50.0F, offset.y), max(-50.0F, offset.z));
	offset = model->GetTransform().GetOffset();
	model->SetTransform(transform);*/

	transform = model->GetTransform();
	offset = model->GetTransform().GetOffset();
	Vector3 newoffset = model->GetTransform().GetOffset();

	float x = offset.x;
	float y = offset.y;
	float z = offset.z;

	targetdirection = Math::Matrix3::FromEuler(dir);
	//40.0F
	if (transform.GetOffset().x > bounds) { newoffset.x = offset.x - ((x - bounds) / bounds); }
	if (transform.GetOffset().y > bounds) { newoffset.y = offset.y - ((y - bounds) / bounds); }
	if (transform.GetOffset().z > bounds) { newoffset.z = offset.z - ((z - bounds) / bounds); }

	if (transform.GetOffset().x < -bounds) { newoffset.x = offset.x - ((x + bounds) / bounds); }
	if (transform.GetOffset().y < -bounds) { newoffset.y = offset.y - ((y + bounds) / bounds); }
	if (transform.GetOffset().z < -bounds) { newoffset.z = offset.z - ((z + bounds) / bounds); }

	//transform.SetOffset(min(40.0F, offset.x), min(40.0F, offset.y), min(40.0F, offset.z));
	//offset = transform.GetOffset();
	//transform.SetOffset(max(-40.0F, offset.x), max(-40.0F, offset.y), max(-40.0F, offset.z));
	
	transform.SetOffset(newoffset);

	offset = model->GetTransform().GetOffset();
	model->SetTransform(transform);
	//model->mDefaultBBox.ApplyTransform(transform);

	//SporeDebugPrint(to_string(offset.x).c_str());
	//SporeDebugPrint(to_string(offset.y).c_str());
	//SporeDebugPrint(to_string(offset.z).c_str());
	// Called on every frame.
	// delta1 and delta2 are the ellapsed time in seconds since the last call,
	// the difference between both is not known.
//	world->UpdateModel(model.get());
	//cam->SetTarget(offset.x,offset.y,offset.z);
	MyGameMode::vehicleoffset = offset;
}

void MyGameMode::SwapBG(uint32_t bg_id,float size,float height, float SpaceHeight)
{
	auto oldbg = background;
	if (background = world->LoadModel(bg_id, GroupIDs::EditorRigblocks))
	{
		//world->DestroyModel(oldbg.get(), true);
		background->SetTransform(background->GetTransform().SetScale(size));
		background->SetTransform(background->GetTransform().SetOffset(0, 0, height)); 
		if (isspaceship == true) { background->SetTransform(background->GetTransform().SetOffset(0, 0, SpaceHeight)); }
		world->UpdateModel(background.get());
	}
	else { background = oldbg; }
}

MyGameMode* MyGameMode::Get()
{
	return sInstance;
}

uint32_t MyGameMode::spaceship = 0x06576dbd;
ResourceKey MyGameMode::selection;

MyGameMode* MyGameMode::sInstance;
Vector3 MyGameMode::vehicleoffset;
UTFWin::UILayout MyGameMode::layout;
uint32_t MyGameMode::prevGameMode = 0;
Editors::EditorModel* MyGameMode::prevEdModel = nullptr;
EditorRequestPtr MyGameMode::editor = nullptr;//new Editors::EditorRequest();
uint32_t MyGameMode::prevEditorGameMode = kGGEMode;
//Editors::EditorModel* prevEdModel = nullptr;

//old movement code

//	Math::Matrix3 newdirection = Matrix3::FromEuler({ 0,0, sin(Math::ToDegrees(targetdirection.ToEuler().y - direction.ToEuler().y)) });
//	model->SetTransform(Transform().Multiply(Transform().SetOffset(targetdirection * Vector3(offset.x+targetoffset.x,offset.y+targetoffset.y,offset.z+targetoffset.z))));
//	model.get()->SetTransform(Transform().SetRotation(targetdirection).SetOffset(offset.x + targetoffset.x, offset.y + targetoffset.y, offset.z + targetoffset.z));

//	if (mInput.IsKeyDown(VK_LEFT))
//	{
//		targetoffset = { 0.03125F,targetoffset.y,targetoffset.z };
//		dir = -90;
//		targetdirection = Matrix3::FromEuler({ 0,0,Math::ToRadians(dir) });
//	}
//	if (mInput.IsKeyDown(VK_RIGHT))
//	{
//		if (dir != -90) targetoffset = { -0.03125F,targetoffset.y,targetoffset.z };
//		else targetoffset = {0, targetoffset.y, targetoffset.z};
//		if (dir == 0){ dir = 90; }
//		else { dir = 0; }
//		targetdirection = Matrix3::FromEuler({ 0,0,Math::ToRadians(dir) });
//	}
//	if (mInput.IsKeyDown(VK_DOWN))
//	{
//		targetoffset = { targetoffset.x,0.03125F,targetoffset.z };
//		if (!mInput.IsKeyDown(VK_UP))
//		{
//			if (dir == 90) { dir = 45; } if (dir == -90) { dir = -45; }
//		}
//		targetdirection = Matrix3::FromEuler({ 0,0,Math::ToRadians(dir) });
//	}
//	if (mInput.IsKeyDown(VK_UP))
//	{
//		targetoffset = { targetoffset.x,-0.03125F,targetoffset.z };
//		if (!mInput.IsKeyDown(VK_DOWN))
//		{
//			if (dir == 90) { dir = -225; } if (dir == -90) { dir = 225; }
//			if (dir == 0) { dir = 180; }
//		}
//		else { targetoffset = { targetoffset.x,0,targetoffset.z };}
//		targetdirection = Matrix3::FromEuler({ 0,0,Math::ToRadians(dir) });
//	}

