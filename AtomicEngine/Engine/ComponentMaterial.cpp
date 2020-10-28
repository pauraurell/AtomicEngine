#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial() : Component()
{
	type = ComponentType::Material;
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
