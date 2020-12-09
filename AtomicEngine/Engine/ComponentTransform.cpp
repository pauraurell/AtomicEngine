#include "ComponentTransform.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"

ComponentTransform::ComponentTransform() : Component()
{
	type = ComponentType::Transform;

	//pos = { 0.0f, 0.0f, 0.0f };
	//rot = { 0.0f, 0.0f, 0.0f , 1.0f};
	//scale = { 1.0f, 1.0f, 1.0f };

	/*transform = float4x4::FromTRS(pos, rot, scale);
	UpdateEulerAngles();*/
}

/*ComponentTransform::ComponentTransform(float3 position, Quat rotation, float3 scale2) : Component()
{
	type = ComponentType::Transform;

	pos = position;
	rot = rotation;
	scale = scale2;

	transform = float4x4::FromTRS(pos, rot, scale);
	UpdateEulerAngles();
}*/

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

float4x4 ComponentTransform::GetTransform()
{
	return transform;
}

float3 ComponentTransform::GetPosition()
{
	return pos;
}

Quat ComponentTransform::GetQuatRotation()
{
	return rot;
}

float3 ComponentTransform::GetEulerRotation()
{
	return rotEuler;
}

float3 ComponentTransform::GetScale()
{
	return scale;
}

/*float4x4 ComponentTransform::GetGlobalTransform()
{
	return global_transform;
}*/

/*float4x4 ComponentTransform::GetGlobalTransformT()
{
	return global_transformT;
}*/

/*float3 ComponentTransform::GetGlobalPosition()
{
	float4x4 global_transform = GetGlobalTransform();
	return float3(global_transform[0][3], global_transform[1][3], global_transform[2][3]);
}*/

/*void ComponentTransform::SetPosition(float3 new_position)
{
	pos = new_position;
	UpdateLocalTransform();
}

void ComponentTransform::SetScale(float3 new_scale)
{
	scale = new_scale;
	UpdateLocalTransform();

	//Getting normals sign
	float result = scale.x * scale.y * scale.z;
	flipped_normals = result >= 0 ? false : true;
}

void ComponentTransform::SetQuatRotation(Quat rotation)
{
	rot = rotation;
	UpdateEulerAngles();
	UpdateLocalTransform();
}*/

/*void ComponentTransform::SetEulerRotation(float3 euler_angles)
{
	float3 delta = (euler_angles - rotEuler) * DEGTORAD;
	Quat quatRot = Quat::FromEulerXYZ(delta.x, delta.y, delta.z);
	rot = rot * quatRot;
	rotEuler = euler_angles;
	UpdateLocalTransform();
}*/

/*void ComponentTransform::SetGlobalTransform(float4x4 transform)
{
	float4x4 localTransform = gameObject->parent->GetComponent<C_Transform>()->GetGlobalTransform().Inverted() * transform;
	this->transform = localTransform;
	global_transform = transform;
	global_transformT = global_transform.Transposed();
	transform_updated = true;
}*/

/*void ComponentTransform::UpdateLocalTransform()
{
	transform = float4x4::FromTRS(pos, rot, scale);
	transform_updated = true;
}

void ComponentTransform::UpdateEulerAngles()
{
	rotEuler = rot.ToEulerXYZ();
	rotEuler *= RADTODEG;
}*/