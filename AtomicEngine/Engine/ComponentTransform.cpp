#include "ComponentTransform.h"

ComponentTransform::ComponentTransform() : Component()
{
	type = ComponentType::Transform;
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
