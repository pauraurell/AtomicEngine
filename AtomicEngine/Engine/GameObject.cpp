#include "GameObject.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ModuleSceneIntro.h"

GameObject::GameObject()
{
	UUID = LCG().Int();
	CreateComponent(ComponentType::Transform);
	active = true;
	name = "GameObject";
	BB = AABB({ 0,0,0 }, { 0,0,0 });
}

GameObject::GameObject(const char* GOname)
{
	UUID = LCG().Int();
	active = true;
	name = GOname;
	CreateComponent(ComponentType::Transform);
	BB = AABB({ 0,0,0 }, { 0,0,0 });
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
		if (App->gui->bbChecker) 
		{ 
			RenderBB(BB);
		}
		if (GetCMesh() != nullptr && GetCMesh()->m != nullptr && GetCMesh()->active == true)
		{
			if (GetCMaterial() != nullptr && GetCMaterial()->tex != nullptr && GetCMaterial()->hasTex == true && GetCMaterial()->active == true)
			{
				App->renderer3D->RenderGameObject(this);
			}
			else { App->renderer3D->RenderGameObject(this); }
		}
	}
}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* newComponent = nullptr;
	switch (type)
	{
	case ComponentType::Transform:
		newComponent = new ComponentTransform(this);
		break;
	case ComponentType::Mesh:
		newComponent = new ComponentMesh(this);
		break;
	case ComponentType::Material:
		newComponent = new ComponentMaterial(this);
		break;
	case ComponentType::Camera:
		newComponent = new ComponentCamera(this);
		break;
	}
	newComponent->owner = this;
	components.push_back(newComponent);

	atLOG("New Component Created");

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
		case ComponentType::Camera:
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
		if (components[i] == comp)
		{ 
			switch (components[i]->type)
			{
			case ComponentType::Mesh:
				for (int j = 0; j < App->scene_intro->meshes.size(); j++)
				{
					if(App->scene_intro->meshes[j] == components[i]->owner->GetCMesh()->m)
					{
						App->scene_intro->meshes.erase(App->scene_intro->meshes.begin() + j);
					}
				}
				break;
			case ComponentType::Material:
				for (int j = 0; j < App->scene_intro->texs.size(); j++)
				{
					if (App->scene_intro->texs[j] == components[i]->owner->GetCMaterial()->tex)
					{
						App->scene_intro->texs.erase(App->scene_intro->texs.begin() + j);
					}
				}

				for (int j = 0; j < App->scene_intro->game_objects.size(); j++)
				{
					if (App->scene_intro->game_objects[j]->GetCMaterial() == components[i])
					{
						delete App->scene_intro->game_objects[j]->GetCMaterial()->tex;
						delete App->scene_intro->game_objects[j]->GetCMaterial();
					}
				}
				break;
			}

			components.erase(components.begin() + i); deleted = true;
		}
	}
	comp = nullptr;
	delete comp;

	atLOG("Component deleted");
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

ComponentCamera* GameObject::GetCCamera()
{
	ComponentCamera* cCamera;

	for (int i = 0; i < components.size(); i++) {
		if (components[i]->type == ComponentType::Camera) {
			cCamera = (ComponentCamera*)components[i];
			return cCamera;
		}
	}
	return nullptr;
}

void GameObject::CreateChild(GameObject* child)
{
	if (child != nullptr)
	{
		children.push_back(child);
	}	
}

void GameObject::CreateNewChild()
{
	GameObject* child = new GameObject();
	if (child != nullptr)
	{
		children.push_back(child);
	}
}

bool GameObject::DeleteChild(GameObject* to_delete)
{
	bool ret = false;
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i] == to_delete)
		{
			children.erase(children.begin() + i);
			ret = true;
		}
	}
	return ret;
}

void GameObject::Reparent(GameObject* new_parent)
{
	if (this->children.size() > 0) 
	{
		for (int i = 0; i < children.size(); i++)
		{
			if (this->children[i] != new_parent)
			{
				parent->DeleteChild(this);
				parent = new_parent;
				new_parent->CreateChild(this);
			}
		}
	}
	else  
	{
		parent->DeleteChild(this);
		parent = new_parent;
		new_parent->CreateChild(this);
	}
}

void GameObject::DeleteChildren()
{
	for (size_t i = 0; i < children.size(); i++){ children[i]->DeleteChildren(); children[i] = nullptr;}
	this->~GameObject();
}

void GameObject::SetRootChild()
{
	this->parent = App->scene_intro->root;
	App->scene_intro->root->CreateChild(this);
}

void GameObject::CalculateBB()
{
	BB.SetNegativeInfinity();

	if (children.empty() == false)
	{
		for (std::vector<GameObject*>::iterator it_c = children.begin(); it_c != children.end(); it_c++)
		{
			(*it_c)->CalculateBB();
			if ((*it_c)->BB.IsFinite()) { BB.Enclose((*it_c)->BB); }
		}
	}

	if (GetCMesh() != nullptr) { BB.Enclose((float3*)GetCMesh()->m->vertex, GetCMesh()->m->num_vertex); }

	if (children.size() <= 0) { BB.TransformAsAABB(GetCTransform()->globalMat); }
}

void GameObject::RenderBB(AABB& BB) 
{
	CalculateBB();

	glDisable(GL_LIGHTING);

	if (this == App->gui->selectedObj)
	{
		glColor3f(0.5, 0.9, 0.6);
		glLineWidth(1.5f);
	}

	else
	{
		glColor3f(0.85, 0.85, 0.85);
		glLineWidth(1.0f);
	}

	glBegin(GL_LINES);

	glVertex3f(BB.CornerPoint(0).x, BB.CornerPoint(0).y, BB.CornerPoint(0).z);
	glVertex3f(BB.CornerPoint(1).x, BB.CornerPoint(1).y, BB.CornerPoint(1).z);
	glVertex3f(BB.CornerPoint(0).x, BB.CornerPoint(0).y, BB.CornerPoint(0).z);
	glVertex3f(BB.CornerPoint(2).x, BB.CornerPoint(2).y, BB.CornerPoint(2).z);

	glVertex3f(BB.CornerPoint(0).x, BB.CornerPoint(0).y, BB.CornerPoint(0).z);
	glVertex3f(BB.CornerPoint(4).x, BB.CornerPoint(4).y, BB.CornerPoint(4).z);
	glVertex3f(BB.CornerPoint(3).x, BB.CornerPoint(3).y, BB.CornerPoint(3).z);
	glVertex3f(BB.CornerPoint(1).x, BB.CornerPoint(1).y, BB.CornerPoint(1).z);

	glVertex3f(BB.CornerPoint(6).x, BB.CornerPoint(6).y, BB.CornerPoint(6).z);
	glVertex3f(BB.CornerPoint(2).x, BB.CornerPoint(2).y, BB.CornerPoint(2).z);
	glVertex3f(BB.CornerPoint(6).x, BB.CornerPoint(6).y, BB.CornerPoint(6).z);
	glVertex3f(BB.CornerPoint(4).x, BB.CornerPoint(4).y, BB.CornerPoint(4).z);

	glVertex3f(BB.CornerPoint(6).x, BB.CornerPoint(6).y, BB.CornerPoint(6).z);
	glVertex3f(BB.CornerPoint(7).x, BB.CornerPoint(7).y, BB.CornerPoint(7).z);
	glVertex3f(BB.CornerPoint(5).x, BB.CornerPoint(5).y, BB.CornerPoint(5).z);
	glVertex3f(BB.CornerPoint(1).x, BB.CornerPoint(1).y, BB.CornerPoint(1).z);

	glVertex3f(BB.CornerPoint(5).x, BB.CornerPoint(5).y, BB.CornerPoint(5).z);
	glVertex3f(BB.CornerPoint(4).x, BB.CornerPoint(4).y, BB.CornerPoint(4).z);
	glVertex3f(BB.CornerPoint(5).x, BB.CornerPoint(5).y, BB.CornerPoint(5).z);
	glVertex3f(BB.CornerPoint(7).x, BB.CornerPoint(7).y, BB.CornerPoint(7).z);

	glVertex3f(BB.CornerPoint(3).x, BB.CornerPoint(3).y, BB.CornerPoint(3).z);
	glVertex3f(BB.CornerPoint(2).x, BB.CornerPoint(2).y, BB.CornerPoint(2).z);
	glVertex3f(BB.CornerPoint(3).x, BB.CornerPoint(3).y, BB.CornerPoint(3).z);
	glVertex3f(BB.CornerPoint(7).x, BB.CornerPoint(7).y, BB.CornerPoint(7).z);

	glEnd();
	glEnable(GL_LIGHTING);
}