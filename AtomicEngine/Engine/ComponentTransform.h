#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentTransform : public Component {
public:
	ComponentTransform(GameObject* go);
	~ComponentTransform();

	void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;

	void SetGlobalTransform(float4x4 trans);
	void UpdateLocalMatrix();
	void UpdateGlobalMatrix();
	void UpdateGlobalMatrix (float4x4 trans);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetRotation(Quat rot);
	void SetScale(float x, float y, float z);
	void SetTransform(float trans[16]);

	float3 pos;
	float3 scale;
	float3 rot;
	Quat rotQuat;

	float4x4 localMat = float4x4::identity;
	float4x4 globalMat = float4x4::identity;
	float4x4 parentGlobalMat = float4x4::identity;
};

#endif //__TRANSFORM_H__