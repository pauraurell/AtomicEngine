#pragma once
#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "ModuleImporter.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	void Enable();
	void Update();
	void Disable();

	Mesh* m;
};

#endif //__COMPONENTMESH_H__
