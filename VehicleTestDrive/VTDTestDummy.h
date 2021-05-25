#pragma once

#include <Spore\BasicIncludes.h>

#define VTDTestDummyPtr intrusive_ptr<VTDTestDummy>

class VTDTestDummy 
	: public Object
	, public DefaultRefCounted
	, public App::IUpdatable
{
	ModelPtr model;
public:
	static const uint32_t TYPE = id("VTDTestDummy");
	
	VTDTestDummy();
	~VTDTestDummy();
	void Load(ResourceKey modelKey, Vector3 position,Vector3 rotation, double hitpoints);
	int AddRef() override;
	void Update() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	double HP = 9999999;

	Vector3 pos;

	Vector3 rot;

};
