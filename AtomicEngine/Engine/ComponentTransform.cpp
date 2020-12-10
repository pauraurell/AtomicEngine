#include "ComponentTransform.h"

ComponentTransform::ComponentTransform() : Component()
{
	type = ComponentType::Transform;

	this->owner = owner;

	pos = float3{ 0,0,0 };
	scale = float3{ 1,1,1 };
	rot = float3{ 0,0,0 };
	rotationQuat = Quat{ 0.0f, 0.0f, 0.0f, 1.0f };

	localMatrix = float4x4::identity;
	globalMatrix = float4x4::identity;
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Enable()
{
	active = true;
}

void ComponentTransform::Update()
{
}

void ComponentTransform::Disable()
{
	active = false;
}

float3 ComponentTransform::GetPosition()
{
	return pos;
}

float3 ComponentTransform::GetScale()
{
	return scale;
}

void ComponentTransform::SetRotation(float x, float y, float z)
{
	rot.x = x;
	rot.y = y;
	rot.z = z;
}

void ComponentTransform::SetQuaternionRotation(Quat quatRot)
{
	rotationQuat = quatRot;
}

void ComponentTransform::SetPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void ComponentTransform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

float3 ComponentTransform::GetEulerRotation()
{
	return rot;
}

Quat ComponentTransform::GetQuaternionRotation()
{
	return rotationQuat;
}

void ComponentTransform::RecalculateMatrix()
{
	localMatrix.Set(float4x4::FromTRS(pos, rotationQuat, scale));
}

