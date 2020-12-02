#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "ImGui/imgui.h"

class Component;
class ComponentMesh;
class ComponentTransform;
class ComponentMaterial;
class ComponentCamera;
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
	ComponentCamera* GetCCamera();


	string name;
	bool active;
	bool is_selected;

	ImGuiTreeNodeFlags_ flag = ImGuiTreeNodeFlags_None;

private:
	vector<Component*> components;

};

#endif //__GAMEOBJECT_H__