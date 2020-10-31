#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial() : Component()
{
	type = ComponentType::Material;
}

ComponentMaterial::ComponentMaterial(At_Tex* text) : Component()
{
	type = ComponentType::Material;
	tex = text;
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
