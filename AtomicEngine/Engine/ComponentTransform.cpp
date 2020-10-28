#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject* c_owner) : Component()
{
	type = ComponentType::Transform;
	owner = c_owner;

	pos.x = 0.0f;	pos.y = 0.0f;	pos.z = 0.0f;
	rot.x = 0.0f;	rot.y = 0.0f;	rot.z = 0.0f;
	scale.x = 0.0f;	scale.y = 0.0f;	scale.z = 0.0f;
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Enable()
{
	active = true;
}

void ComponentTransform::Update()
{

}

void ComponentTransform::Disable()
{
	active = false;
}
