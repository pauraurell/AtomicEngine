#pragma once
#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	void Enable();
	void Update();
	void Disable();
};

#endif //__COMPONENTMESH_H__
