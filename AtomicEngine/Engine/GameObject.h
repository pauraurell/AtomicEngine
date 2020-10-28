#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>

class Component;
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

	const char* name;
	bool active;

private:
	vector<Component*> components;

};

#endif //__GAMEOBJECT_H__