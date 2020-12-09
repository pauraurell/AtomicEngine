#pragma once
#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Component.h"
#include "glmath.h"
#include "MathGeoLib\include\MathBuildConfig.h"
#include "MathGeoLib\include\MathGeoLib.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	//ComponentCamera();
	~ComponentCamera();

	void Enable();
	void Update();
	void Disable();

	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetFOV() const;
	float GetAspectRatio() const;

	void SetNearPlane(float distance);
	void SetFarPlane(float distance);
	void SetFOV(float fov);
	void SetAspectRatio(float ar);

	void Look(const float3& position);
	void Match(const ComponentCamera* reference);

	float* GetOpenGLViewMatrix();
	float* GetOpenGLProjectionMatrix();

	void OnUpdateTransform(const float4x4& global, const float4x4& parent_global = float4x4::identity);

	/*void Save();
	void Load();*/

private:
	void UpdatePlanes();

public:
	Frustum		frustum;
	bool		update_projection = true;
	bool		culling = false;
	bool		active_camera = false;
	Plane		planes[6];
};

#endif //__COMPONENTCAMERA_H__
