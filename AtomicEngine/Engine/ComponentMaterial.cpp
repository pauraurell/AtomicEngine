#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* go) : Component()
{
	type = ComponentType::Material;
	owner = go;
	hasTex = false;
}

ComponentMaterial::ComponentMaterial(GameObject* go, At_Tex* text) : Component()
{
	type = ComponentType::Material;
	owner = go;
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
