#pragma once
#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* go);
	~ComponentTransform();

	void Enable();
	void Update();
	void Disable();

	float3 GetPosition();
	float3 GetScale();
	float3 GetEulerRotation();
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetQuaternionRotation(Quat quadRot);
	Quat GetQuaternionRotation();
	void RecalculateMatrix();

public:
	float3 pos = { 23,0,0 };
	float3 rot = { 0.0f,0.0f,0.0f };
	float3 scale = { 1.0f,1.0f,1.0f };
	Quat rotationQuat = { 0.0f,0.0f,0.0f,1.0f };

	float4x4 localMatrix = float4x4::identity;
	float4x4 globalMatrix = float4x4::identity;
};

#endif //__COMPONENTTRANSFORM_H__
