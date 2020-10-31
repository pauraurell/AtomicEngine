#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "ModuleImporter.h"
#include "Texture.h"


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	GameObject* CreateGameObject();
	GameObject* CreateGameObject(Mesh* m);
	GameObject* CreateGameObject(Mesh* m, string name);
	void DeleteGameObject(GameObject* to_delete);
	vector<GameObject*> game_objects;
	vector<Mesh*> meshes;
	vector<At_Tex*> texs;

public:

	bool grid;
};
