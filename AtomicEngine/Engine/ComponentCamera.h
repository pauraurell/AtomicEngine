#pragma once
#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

enum FixedFOV {
	FIXED_VERTICAL_FOV,
	FIXED_HORIZONTAL_FOV
};

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* go);
	//ComponentCamera();
	~ComponentCamera();

	void Enable();
	void Update();
	void Disable();

	void SetFixedFOV(FixedFOV fixedFOV);
	void AdjustFieldOfView();
	void AdjustFieldOfView(float width, float height);
	void SetVerticalFieldOfView(float verticalFOV, float screen_width, float screen_height);
	void SetHorizontalFieldOfView(float horizontalFOV, float screen_width = 16.0f, float screen_height = 9.0f);
	void SetPosition(float3 position);
	void SetReference(float3 reference);
	void SetNearPlaneDistance(float distance);
	void SetFarPlaneDistance(float distance);

	void Look(float3 spot);
	Frustum GetFrustum();

	float* GetViewMatrix();
	float* GetProjectionMatrix();
	bool ContainsAABB(AABB& aabb);

public:
	FixedFOV fixedFOV;

	Frustum frustum;
	float aspectRatio;
	float3 reference;
};

#endif //__COMPONENTCAMERA_H__
