#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "GameObject.h"
#include "MathGeoLib/include/Math/float3.h"

enum class ComponentType
{
	Transform,
	Mesh,
	Material,
	Camera,
	NoType
};

class Component
{
public:
	Component();
	~Component();

	virtual void Enable()
	{}
	virtual void Update()
	{}
	virtual void Disable()
	{}

	GameObject* owner;
	ComponentType type;
	bool active;
};
#endif //__COMPONENT_H__