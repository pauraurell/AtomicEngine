#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* c_owner) : Component()
{
	type = ComponentType::Material;
	owner = c_owner;
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
