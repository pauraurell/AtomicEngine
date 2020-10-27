#include "GameObject.h"
#include "Component.h"

GameObject::GameObject()
{
	active = true;
	name = "GameObject";
}


GameObject::~GameObject()
{

}

void GameObject::Draw()
{
	
}

void GameObject::Update()
{

}

Component* CreateComponent(ComponentType type)
{

}
