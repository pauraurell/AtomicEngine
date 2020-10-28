#include "GameObject.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ModuleSceneIntro.h"

GameObject::GameObject()
{
	active = true;
	name = "GameObject";
	is_selected = false;
}

GameObject::GameObject(const char* GOname)
{
	active = true;
	name = GOname;
	is_selected = false;
}

GameObject::~GameObject()
{

}

void GameObject::Update()
{
	for (int i = 0; i < components.size(); ++i)
	{
		Component* component = components[i];
		component->Update();	
	}
}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* newComponent = nullptr;

	switch (type)
	{
	case ComponentType::Transform:
		newComponent = new ComponentTransform();
		break;
	case ComponentType::Mesh:
		newComponent = new ComponentMesh();
		break;
	case ComponentType::Material:
		newComponent = new ComponentMaterial();
		break;
	}
	newComponent->owner = this;
	components.push_back(newComponent);

	return newComponent;
}

void GameObject::DeleteComponents()
{
	for (int i = 0; i < components.size(); i++)
	{
		switch (components[i]->type)
		{
		case ComponentType::Mesh:

			break;
		case ComponentType::Transform:

			break;
		case ComponentType::Material:

			break;
		}
	}
	components.clear();
}

ComponentMesh* GameObject::GetCMesh()
{
	ComponentMesh* cMesh;

	for (int i = 0; i < components.size(); i++) {
		if (components[i]->type == ComponentType::Mesh) {
			cMesh = (ComponentMesh*)components[i];
		}
	}

	return cMesh;
}

ComponentTransform* GameObject::GetCTransform()
{
	ComponentTransform* cTransform;

	for (int i = 0; i < components.size(); i++) {
		if (components[i]->type == ComponentType::Transform) {
			cTransform = (ComponentTransform*)components[i];
		}
	}

	return cTransform;
}

ComponentMaterial* GameObject::GetCMaterial()
{
	ComponentMaterial* cMaterial;

	for (int i = 0; i < components.size(); i++) {
		if (components[i]->type == ComponentType::Material) {
			cMaterial = (ComponentMaterial*)components[i];
		}
	}

	return cMaterial;
}
