#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentTransform : public Component {
public:
	ComponentTransform();
	~ComponentTransform();

	void Set(float4x4 transform);

	float4x4 GetLocalTransform();
	float4x4 GetGlobalTransform();
	void SetGlobalTransform(float4x4 trans);

	void UpdateLocalTransform();
	void UpdateGlobalTransform();
	void UpdateGlobalTransform(float4x4 trans);
	void ChangeParentTransform(float4x4 trans);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetRotation(Quat rot);
	void SetScale(float x, float y, float z);

	float4x4 localMat = float4x4::identity;
	float4x4 globalMat = float4x4::identity;
	float4x4 parentGlobalMat = float4x4::identity;

	float3 pos;
	Quat rot;
	float3 scale;
	float3 eulerRotation;
};

#endif //__TRANSFORM_H__