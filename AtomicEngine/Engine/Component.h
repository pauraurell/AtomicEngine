#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "GameObject.h"
#include "MathGeoLib/Math/float3.h"

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

	virtual void Save(GnJSONArray& save_array)
	{};
	virtual void Load(GnJSONObj& load_object)
	{};

	GameObject* owner;
	ComponentType type;
	bool active;
};
#endif //__COMPONENT_H__