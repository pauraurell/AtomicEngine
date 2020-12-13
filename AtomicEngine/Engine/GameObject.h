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
class ComponentCamera;
enum class ComponentType;
class GnJSONObj;
class GnJSONArray;

using namespace std;

class GameObject
{
public:
	GameObject();
	GameObject(const char* GOname);
	~GameObject();

	void Update();
	void Save(GnJSONArray& save_array);
	uint Load(GnJSONObj* object);

	Component* CreateComponent(ComponentType type);
	void DeleteComponent(Component* comp);
	void DeleteComponents();

	ComponentMesh* GetCMesh();
	ComponentTransform* GetCTransform();
	ComponentMaterial* GetCMaterial();
	ComponentCamera* GetCCamera();

	void Reparent(GameObject* new_parent);
	void CreateChild(GameObject* child);
	void CreateNewChild();
	bool DeleteChild(GameObject* to_delete);
	void DeleteChildren();
	void SetRootChild();
	void ChildrenTransform();

	string name;
	bool active;
	uint UUID = 0;

	AABB BB;
	void RenderBB(AABB& BB);
	void CalculateBB();

	ImGuiTreeNodeFlags_ flag = ImGuiTreeNodeFlags_None;

	std::vector<GameObject*> children;
	GameObject* parent = nullptr;

private:
	vector<Component*> components;

};

#endif //__GAMEOBJECT_H__