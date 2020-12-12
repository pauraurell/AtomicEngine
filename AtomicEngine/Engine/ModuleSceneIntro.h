#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "ModuleImporter.h"
#include "Texture.h"
#include "MathGeoLib/MathGeoLib.h"
#include "ImGuizmo/ImGuizmo.h"


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	GameObject* CreateGameObject(bool camera = false);
	GameObject* CreateGameObject(Mesh* m);
	GameObject* CreateGameObject(Mesh* m, string name);
	GameObject* CreateGameObject(Mesh* m, string name, float3 pos, Quat rot, float3 scale);

	void DeleteGameObject(GameObject* to_delete);

	vector<GameObject*> GetGameObjects();
	void PushbackGameObjects(GameObject* gameObject, std::vector<GameObject*>& gameObjects);

	vector<GameObject*> game_objects;
	vector<Mesh*> meshes;
	vector<At_Tex*> texs;

	//void DrawGuizmo(ImGuizmo::OPERATION operation);
	//ImGuizmo::OPERATION guizmoOperation;

	GameObject* root;
	bool grid;
};
