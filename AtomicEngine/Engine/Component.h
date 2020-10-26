#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class Component
{
public:
	enum ComponentType
	{
		Transform,
		Mesh,
		Material,
		NoType
	};

public:
	Component();
	~Component();

	ComponentType type;
	bool active;
};
#endif //__COMPONENT_H__