#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentTransform : public Component {
public:
	ComponentTransform();
	~ComponentTransform();

	void SetGlobalTransform(float4x4 trans);
	void UpdateLocalMatrix();
	void UpdateGlobalMatrix();
	void UpdateGlobalMatrix (float4x4 trans);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetRotation(Quat rot);
	void SetScale(float x, float y, float z);

	float3 pos;
	float3 scale;
	float3 rot;
	Quat rotQuat;

	float4x4 localMat = float4x4::identity;
	float4x4 globalMat = float4x4::identity;
	float4x4 parentGlobalMat = float4x4::identity;
};

#endif //__TRANSFORM_H__