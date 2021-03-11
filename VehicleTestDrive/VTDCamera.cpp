#include "stdafx.h"
#include "VTDCamera.h"
#include "MyGameMode.h"

VTDCamera::VTDCamera()
	: mInput()
	, mDistance(4.0f)
	, mAngleX(0)
	, mAngleY(0)
	, mTarget(0, 0, 0)
	, mRotateSpeed(PI)
{
}

VTDCamera::~VTDCamera()
{

}

void VTDCamera::SetTarget(float targetx, float targety, float targetz)
{
	mTarget.x = targetx;
	mTarget.y = targety;
	mTarget.z = targetz;
}

// For internal use, do not touch this.
int VTDCamera::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not touch this.
int VTDCamera::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* VTDCamera::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(ICamera);
	CLASS_CAST(VTDCamera);
	return nullptr;
}


bool VTDCamera::OnAttach(App::ICameraManager* pManager) {
	return true;
}

bool VTDCamera::OnDeattach() {
	return true;
}

void VTDCamera::OnEnter() {

}

void VTDCamera::OnExit() {

}

/// UPDATE FUNCTION ///
void VTDCamera::Update(int deltaTime, App::cViewer* pViewer) {
	// Called every frame.
	// delta1 is the ellapsed time in milliseconds since the last call
	pViewer->SetFarPlane(1000.0f);
	pViewer->SetNearPlane(0.01f);

	if (mInput.IsMouseDown(MouseButton::kMouseButtonLeft)) {
		Point deltaMouse = mInput.mousePosition - mLastMouse;
		

		mAngleX -= deltaMouse.x * mRotateSpeed / pViewer->GetViewport().Width;
		mAngleY += deltaMouse.y * mRotateSpeed / pViewer->GetViewport().Height;

		mAngleY = max(min(mAngleY, PI * 0.95f / 2.0f), -PI * 0.95f / 2.0f);
	}
	mLastMouse = mInput.mousePosition;

	// Spherical coordinates
	float colatitude = Math::PI / 2.0f - mAngleY;
	Vector3 position = {
		mDistance * cosf(mAngleX) * sinf(colatitude),
		mDistance * sinf(mAngleX) * sinf(colatitude),
		mDistance * cosf(colatitude)
	};

	mTarget = MyGameMode::vehicleoffset;
	pViewer->SetCameraTransform(Transform().SetOffset(mTarget+position).SetRotation(Matrix3::LookAt(position, Vector3(0,0,0))));
	};

void VTDCamera::func24h(bool arg) {
	if (!arg) mInput.Reset();
}


//// INPUT LISTENERS ////

// Called when a keyboard key button is pressed.
bool VTDCamera::OnKeyDown(int virtualKey, KeyModifiers modifiers)
{
	mInput.OnKeyDown(virtualKey, modifiers);
	
	// Return true if the keyboard event has been handled in this method.
	return false;
}

// Called when a keyboard key button is released.
bool VTDCamera::OnKeyUp(int virtualKey, KeyModifiers modifiers)
{
	mInput.OnKeyUp(virtualKey, modifiers);
	
	// Return true if the keyboard event has been handled in this method.
	return false;
}

// Called when a mouse button is pressed (this includes the mouse wheel button).
bool VTDCamera::OnMouseDown(MouseButton mouseButton, float mouseX, float mouseY, MouseState mouseState)
{
	mInput.OnMouseDown(mouseButton, mouseX, mouseY, mouseState);
	
	// Return true if the mouse event has been handled in this method.
	return false;
}

// Called when a mouse button is released (this includes the mouse wheel button).
bool VTDCamera::OnMouseUp(MouseButton mouseButton, float mouseX, float mouseY, MouseState mouseState)
{
	mInput.OnMouseUp(mouseButton, mouseX, mouseY, mouseState);
	
	// Return true if the mouse event has been handled in this method.
	return false;
}

// Called when the mouse is moved.
bool VTDCamera::OnMouseMove(float mouseX, float mouseY, MouseState mouseState)
{
	mInput.OnMouseMove(mouseX, mouseY, mouseState);
	
	// Return true if the mouse event has been handled in this method.
	return false;
}

// Called when the mouse wheel is scrolled. 
// This method is not called when the mouse wheel is pressed.
bool VTDCamera::OnMouseWheel(int wheelDelta, float mouseX, float mouseY, MouseState mouseState)
{
	mInput.OnMouseWheel(wheelDelta, mouseX, mouseY, mouseState);

	mDistance -= (wheelDelta / (120 * 4.0f)) * max(0.5F,(mDistance/2.25F));
	if (mDistance < 0) { mDistance = 0.1; }
	mDistance = min(mDistance, 50.0F);

	return false;
	// Return true if the mouse event has been handled in this method.
	return false;
}


bool VTDCamera::func40h(int) {
	return false;
}
bool VTDCamera::func44h(int) {
	return false;
}
bool VTDCamera::func48h(int) {
	return false;
}

App::PropertyList* VTDCamera::GetPropertyList() {
	return nullptr;
}

void VTDCamera::Initialize() {
	VTDCamera::sInstance = this;
}

void VTDCamera::func54h(Vector3& dst) {
	dst = { 0, 0, 0 };
}

VTDCamera* VTDCamera::Get()
{
	return sInstance;
}

VTDCamera* VTDCamera::sInstance;