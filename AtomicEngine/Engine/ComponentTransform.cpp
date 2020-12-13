#include "ComponentTransform.h"
#include "ImGui/imgui.h"
#include "Globals.h"
#include "GameObject.h"
#include "ModuleJSON.h"

ComponentTransform::ComponentTransform(GameObject* go) : Component()
{
	type = ComponentType::Transform;
	owner = go;

	pos = float3::zero; rotQuat = Quat::identity;
	rot = float3::zero; scale = float3::one;
	
	localMat = float4x4::FromTRS(pos, rotQuat, scale);
	globalMat = localMat;

	parentGlobalMat = float4x4::identity;
}

ComponentTransform::~ComponentTransform() {}

void ComponentTransform::SetGlobalTransform(float4x4 trans)
{
	globalMat = trans;

	float4x4 inv = parentGlobalMat;
	inv.Inverse();
	localMat = inv * globalMat;
	
	localMat.Decompose(pos, rotQuat, scale);
	rot = rotQuat.ToEulerXYZ();
	rot *= RADTODEG;
}

void ComponentTransform::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;
	save_object.AddInt("Type", (int)type);

	save_object.AddFloat3("Position", pos);
	save_object.AddFloat3("Rotation", rot);
	save_object.AddQuaternion("Rotation Quat", rotQuat);
	save_object.AddFloat3("Scale", scale);

	save_array.AddObject(save_object);
}

void ComponentTransform::Load(GnJSONObj& load_object)
{
	pos = load_object.GetFloat3("Position");
	rot = load_object.GetFloat3("Rotation");
	rotQuat = load_object.GetQuaternion("Rotation Quat");
	scale = load_object.GetFloat3("Scale");

	UpdateGlobalMatrix();
}

void ComponentTransform::UpdateLocalMatrix()
{
	localMat = float4x4::FromTRS(pos, rotQuat, scale);
	rot = rotQuat.ToEulerXYZ();
	rot *= RADTODEG;

	if (owner->children.size() > 0) 
	{
		for (size_t i = 0; i < owner->children.size(); i++)
		{
			owner->children[i]->GetCTransform()->UpdateGlobalMatrix(globalMat);
		}
	}
}

void ComponentTransform::UpdateGlobalMatrix()
{
	UpdateLocalMatrix();
	globalMat = parentGlobalMat * localMat;
}

void ComponentTransform::UpdateGlobalMatrix(float4x4 parentTrans)
{
	UpdateLocalMatrix();
	parentGlobalMat = parentTrans;
	globalMat = parentGlobalMat * localMat;
}

void ComponentTransform::SetPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
	UpdateLocalMatrix();
}

void ComponentTransform::SetRotation(float x, float y, float z)
{
	rot = float3(x, y, z);
	rotQuat = Quat::FromEulerXYZ(x * DEGTORAD, y * DEGTORAD, z * DEGTORAD);
	UpdateGlobalMatrix();
}

void ComponentTransform::SetRotation(Quat rotationQuat)
{
	rotQuat = rotationQuat;
	rot = rotQuat.ToEulerXYZ() * RADTODEG;
	UpdateGlobalMatrix();
}

void ComponentTransform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	UpdateGlobalMatrix();
}

void ComponentTransform::SetTransform(float trans[16])
{
	float4x4 transform;
	transform.Set(trans);
	transform.Transpose();
	SetGlobalTransform(transform);
}