#pragma once
#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Texture.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* go);
	ComponentMaterial(GameObject* go, At_Tex* text);
	~ComponentMaterial();

	void Enable();
	void Update();
	void Disable();

	void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;

	At_Tex* tex;
	bool hasTex;
};

#endif //__COMPONENTMATERIAL_H__
