#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>

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
	void DeleteComponents();
	ComponentMesh* GetCMesh();
	ComponentTransform* GetCTransform();
	ComponentMaterial* GetCMaterial();

	const char* name;
	bool active;

private:
	vector<Component*> components;

};

#endif //__GAMEOBJECT_H__