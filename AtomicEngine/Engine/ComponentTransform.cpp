#include "ComponentTransform.h"
#include "ImGui/imgui.h"
#include "Globals.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform() : Component()
{
	type = ComponentType::Transform;

	pos = float3::zero;
	rot = Quat::identity;
	eulerRotation = float3::zero;
	scale = float3::one;

	localMat = float4x4::FromTRS(pos, rot, scale);
	globalMat = localMat;
	parentGlobalMat = float4x4::identity;
}

ComponentTransform::~ComponentTransform() {}


void ComponentTransform::Set(float4x4 g_transform)
{
	localMat = g_transform;
}

float4x4 ComponentTransform::GetLocalTransform()
{
	return localMat;
}

float4x4 ComponentTransform::GetGlobalTransform()
{
	return globalMat;
}

void ComponentTransform::SetGlobalTransform(float4x4 newTransform)
{
	globalMat = newTransform;
	float4x4 inverseParentGlobal = parentGlobalMat;
	inverseParentGlobal.Inverse();
	localMat = inverseParentGlobal * globalMat;
	UpdateTRS();
	owner->UpdateChildrenTransforms();
}

void ComponentTransform::UpdateLocalTransform()
{
	localMat = float4x4::FromTRS(pos, rot, scale);
	UpdateEulerRotation();
}

void ComponentTransform::UpdateTRS()
{
	localMat.Decompose(pos, rot, scale);
	UpdateEulerRotation();
}

void ComponentTransform::UpdateGlobalTransform()
{
	UpdateLocalTransform();
	globalMat = parentGlobalMat * localMat;
}

void ComponentTransform::UpdateGlobalTransform(float4x4 parentGlobalTransform)
{
	UpdateLocalTransform();
	parentGlobalMat = parentGlobalTransform;
	globalMat = parentGlobalMat * localMat;
}

void ComponentTransform::ChangeParentTransform(float4x4 newParentGlobalTransform)
{
	parentGlobalMat = newParentGlobalTransform;
	newParentGlobalTransform.Inverse();
	localMat = newParentGlobalTransform * globalMat;
	UpdateTRS();
	globalMat = parentGlobalMat * localMat;
}

void ComponentTransform::SetPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;

	UpdateLocalTransform();
}

void ComponentTransform::SetRotation(float x, float y, float z)
{
	eulerRotation = float3(x, y, z);
	rot = Quat::FromEulerXYZ(x * DEGTORAD, y * DEGTORAD, z * DEGTORAD);
	UpdateGlobalTransform();
}

void ComponentTransform::SetRotation(Quat newrot)
{
	rot = newrot;
	eulerRotation = rot.ToEulerXYZ() * RADTODEG;

	UpdateGlobalTransform();
}

Quat ComponentTransform::GetRotation()
{
	return rot;
}

void ComponentTransform::UpdateEulerRotation()
{
	eulerRotation = rot.ToEulerXYZ();
	eulerRotation *= RADTODEG;
}

void ComponentTransform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	UpdateGlobalTransform();
}