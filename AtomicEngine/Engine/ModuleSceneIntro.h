#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "ModuleImporter.h"


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	GameObject* CreateGameObject();
	GameObject* CreateGameObject(mesh* m);
	GameObject* CreateGameObject(mesh* m, string name);
	void DeleteGameObject(GameObject* to_delete);
	vector<GameObject*> game_objects;
	vector<mesh*> meshes;

public:

	bool grid;
};
