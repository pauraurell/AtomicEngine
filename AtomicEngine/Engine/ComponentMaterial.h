#pragma once
#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Texture.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	ComponentMaterial(At_Tex* text);
	~ComponentMaterial();

	void Enable();
	void Update();
	void Disable();

	At_Tex* tex;
};

#endif //__COMPONENTMATERIAL_H__
