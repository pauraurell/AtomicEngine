#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial() : Component()
{
	type = ComponentType::Material;
	hasTex = false;
}

ComponentMaterial::ComponentMaterial(At_Tex* text) : Component()
{
	type = ComponentType::Material;
	tex = text;
	hasTex = true;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Enable()
{
	active = true;
}

void ComponentMaterial::Update()
{
}

void ComponentMaterial::Disable()
{
	active = false;
}
