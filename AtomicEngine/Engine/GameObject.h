#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "ImGui/imgui.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Globals.h"

class Component;
class ComponentMesh;
class ComponentTransform;
class ComponentMaterial;
enum class ComponentType;

using namespace std;

class GameObject
{
public:
	GameObject();
	GameObject(const char* GOname);
	~GameObject();

	void Update();
	Component* CreateComponent(ComponentType type);
	void DeleteComponent(Component* comp);
	void DeleteComponents();

	ComponentMesh* GetCMesh();
	ComponentTransform* GetCTransform();
	ComponentMaterial* GetCMaterial();

	void Reparent(GameObject* new_parent);
	void CreateChild(GameObject* child);
	void CreateNewChild();
	bool DeleteChild(GameObject* to_delete);
	void DeleteChildren();
	void SetRootChild();

	string name;
	bool active;

	AABB BB;
	void RenderBB(AABB& BB);
	void CalculateBB();

	ImGuiTreeNodeFlags_ flag = ImGuiTreeNodeFlags_None;

	std::vector<GameObject*> children;
	GameObject* parent;


private:
	vector<Component*> components;

};

#endif //__GAMEOBJECT_H__