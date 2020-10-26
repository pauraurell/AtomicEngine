#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "Component.h"

using namespace std;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Draw();

	string name;
	bool active;

private:
	vector<Component*> components;

};

#endif