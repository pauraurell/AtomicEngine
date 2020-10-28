#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

#define BOUNCER_TIME 200

struct PhysBody3D;
class Cube;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init();
	update_status Update();
	bool CleanUp();

	GameObject* CreateGameObject();
	vector<GameObject*> game_objects;

public:

	bool grid;
};
