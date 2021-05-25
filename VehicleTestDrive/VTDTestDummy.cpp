#include "stdafx.h"
#include "VTDTestDummy.h"
#include "MyGameMode.h"

VTDTestDummy::VTDTestDummy()
{
}


VTDTestDummy::~VTDTestDummy()
{
}

void VTDTestDummy::Load(ResourceKey modelKey, Vector3 position, Vector3 rotation, double hitpoints)
{
	if (model = MyGameMode::Get()->world->LoadModel(modelKey.instanceID, modelKey.groupID))
	{
		MyGameMode::Get()->world->SetFixedLod(model.get(), 0);
		pos = position;
		rot = rotation;
		HP = hitpoints;
		model->mFlags |= Graphics::kModelFlagHighRes;

		MyGameMode::Get()->world->UpdateModel(model.get());
		auto transformm = model->GetTransform();

		transformm.SetOffset(position);
		transformm.SetRotation(Math::Matrix3::FromEuler(rotation));

		model->SetTransform(transformm);

		App::AddUpdateFunction(this);
	}
	else
	{
		SporeDebugPrint("WHYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYy");
	}
}

void VTDTestDummy::Update()
{
	pos.z = 0;

	auto transformm = model->GetTransform();

	transformm.SetOffset(pos);
	transformm.SetRotation(Math::Matrix3::FromEuler(rot));
	
	//model->mDefaultBBox.ApplyTransform(transformm);

	model->SetTransform(transformm);

	auto mode = MyGameMode::Get();
	SporeDebugPrint(to_string(HP).c_str());
	//model->mDefaultBBox.ApplyTransform(model->GetTransform());
	vector<Graphics::Model*> models{};
	mode->world->IntersectBBox(mode->model->mDefaultBBox, models);

	bool hasCollided = 0;
	for (int i = 0; i < models.size(); i++)
	{
		if (models[i] == model.get()) { hasCollided = 1; break; }
	}
	
	if (hasCollided == 1)//(model->mDefaultBBox.Contains(mode->model->mDefaultBBox.GetCenter()))
	{
		//pos.x += (pos.x - world->model->GetTransform().GetOffset().x);
		//pos.y += (pos.y - world->model->GetTransform().GetOffset().y);
		//pos.z += (pos.z - world->model->GetTransform().GetOffset().z);
		SporeDebugPrint("YESSSSSSS");
		//mode->model->GetTransform().SetOffset(mode->oldtransform.GetOffset());
	}
	else
	{
		SporeDebugPrint(to_string(model->mDefaultBBox.GetCenter().x).c_str());
		SporeDebugPrint(to_string(model->mDefaultBBox.GetCenter().y).c_str());
		SporeDebugPrint(to_string(model->mDefaultBBox.GetCenter().z).c_str());


		SporeDebugPrint(to_string(mode->model->mDefaultBBox.GetCenter().x).c_str());
		SporeDebugPrint(to_string(mode->model->mDefaultBBox.GetCenter().y).c_str());
		SporeDebugPrint(to_string(mode->model->mDefaultBBox.GetCenter().z).c_str());
	}
}

// For internal use, do not modify.
int VTDTestDummy::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int VTDTestDummy::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* VTDTestDummy::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(VTDTestDummy);
	return nullptr;
}
