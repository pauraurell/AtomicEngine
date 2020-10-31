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
	if (active == true)
	{
		if (GetCMesh() != nullptr && GetCMesh()->m != nullptr && GetCMesh()->active == true)
		{
			if (GetCMaterial() != nullptr && GetCMaterial()->tex != nullptr && GetCMaterial()->active == true)
			{
				App->renderer3D->RenderGameObject(GetCMesh()->m, GetCMaterial()->tex);
			}
			else { App->renderer3D->RenderGameObject(GetCMesh()->m); }
		}
	}
}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* newComponent = nullptr;
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type) { return nullptr; }
	}
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
			DeleteComponent(components[i]);
			break;
		case ComponentType::Transform:
			DeleteComponent(components[i]);
			break;
		case ComponentType::Material:
			DeleteComponent(components[i]);
			break;
		}
	}
	components.clear();
}

void GameObject::DeleteComponent(Component* comp)
{
	bool deleted = false;
	for (int i = 0; i < components.size() && deleted == false; i++)
	{
		if (components[i] == comp) { components.erase(components.begin() + i); deleted = true; }
	}
	delete comp;

	LOG("Component deleted");
}

ComponentMesh* GameObject::GetCMesh()
{
	ComponentMesh* cMesh;

	for (int i = 0; i < components.size(); i++) {
		if (components[i]->type == ComponentType::Mesh) {
			cMesh = (ComponentMesh*)components[i];
			return cMesh;
		}
	}
	return nullptr;
}

ComponentTransform* GameObject::GetCTransform()
{
	ComponentTransform* cTransform;

	for (int i = 0; i < components.size(); i++) {
		if (components[i]->type == ComponentType::Transform) {
			cTransform = (ComponentTransform*)components[i];
			return cTransform;
		}
	}
	return nullptr;
}

ComponentMaterial* GameObject::GetCMaterial()
{
	ComponentMaterial* cMaterial;

	for (int i = 0; i < components.size(); i++) {
		if (components[i]->type == ComponentType::Material) {
			cMaterial = (ComponentMaterial*)components[i];
			return cMaterial;
		}
	}
	return nullptr;
}
