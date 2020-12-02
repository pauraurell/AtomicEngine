#include "ComponentCamera.h"

ComponentCamera::ComponentCamera() : Component()
{
	type = ComponentType::Camera;
}

/*ComponentCamera::ComponentCamera() : Component()
{
	type = ComponentType::Camera;
}*/

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Enable()
{
	active = true;
}

void ComponentCamera::Update()
{
}

void ComponentCamera::Disable()
{
	active = false;
}
