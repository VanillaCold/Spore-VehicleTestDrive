#pragma once

#include <Spore\BasicIncludes.h>
#include <Spore\Swarm\IEffectWorld.h>
#include "VTDCamera.h"
#include "VTDExitButton.h"

#define MyGameModePtr intrusive_ptr<MyGameMode>

class MyGameMode
	: public App::IGameMode
	, public DefaultRefCounted
{
	UTFWin::Window* window;
	IModelWorldPtr world;
	ModelPtr model;
	ModelPtr background;
	ModelPtr background1;
	ModelPtr background2;
	VTDCameraPtr cam;
	
	int currentbackground = 0;
	bool isspaceship = false;
	float friction = 0.0F;
	Graphics::ILightingWorld* lightworld;

	Vector3 targetoffset = {0, 0, 0};
	Math::Matrix3 targetdirection = Matrix3().SetIdentity();
	IEffectWorldPtr chocicetdeffectworld;

public:
	MyGameMode();

	int AddRef() override;
	int Release() override;
	~MyGameMode();

	bool func0Ch() override;
	bool Initialize(App::IGameModeManager* pManager) override;
	bool Dispose() override;
	bool OnEnter() override;
	void OnExit() override;
	void* func20h(int) override;

	bool OnKeyDown(int virtualKey, KeyModifiers modifiers) override;
	bool OnKeyUp(int virtualKey, KeyModifiers modifiers) override;
	bool OnMouseDown(MouseButton mouseButton, float mouseX, float mouseY, MouseState mouseState) override;
	bool OnMouseUp(MouseButton mouseButton, float mouseX, float mouseY, MouseState mouseState) override;
	bool OnMouseMove(float mouseX, float mouseY, MouseState mouseState) override;
	bool OnMouseWheel(int wheelDelta, float mouseX, float mouseY, MouseState mouseState) override;
	void Update(float delta1, float delta2) override;

	void SwapBG(uint32_t bg_id,float size,float height, float SpaceHeight);

	static uint32_t prevGameMode;
	static uint32_t spaceship;
	static ResourceKey selection;
	static Vector3 vehicleoffset;
	static UTFWin::UILayout layout;
	static MyGameMode* Get();
protected:
	GameInput mInput;
	static MyGameMode* sInstance;
};
