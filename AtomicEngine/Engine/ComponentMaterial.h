#pragma once
#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	void Enable();
	void Update();
	void Disable();
};

#endif //__COMPONENTMATERIAL_H__
