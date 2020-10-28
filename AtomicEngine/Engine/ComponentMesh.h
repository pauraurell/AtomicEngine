#pragma once
#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "ModuleImporter.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* owner);
	~ComponentMesh();

	void Enable();
	void Update();
	void Disable();

	mesh* m;
};

#endif //__COMPONENTMESH_H__
