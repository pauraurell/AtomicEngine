#pragma once
#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	ComponentTransform(float3 position, Quat rotation, float3 scale2);
	~ComponentTransform();

	float4x4 GetTransform();
	float3 GetPosition();
	Quat GetQuatRotation();
	float3 GetEulerRotation();
	float3 GetScale();
	//float4x4 GetGlobalTransform();
	//float4x4 GetGlobalTransformT();
	//float3 GetGlobalPosition();

	void SetPosition(float3 position);
	void SetScale(float3 scale);
	void SetQuatRotation(Quat rotation);
	void SetEulerRotation(float3 euler_angles);
	//void SetGlobalTransform(float4x4 transform);

	void UpdateLocalTransform();
	void UpdateEulerAngles();

	void Enable();
	void Update();
	void Disable();

public:
	float3 pos;
	Quat rot;
	float3 rotEuler;
	float3 scale;

	float4x4 transform;

	bool flipped_normals = false;
	bool transform_updated = true;
};

#endif //__COMPONENTTRANSFORM_H__
