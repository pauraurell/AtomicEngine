#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "ModuleImporter.h"
#include "Texture.h"
#include "ComponentCamera.h"
#include "MathGeoLib/MathGeoLib.h"
#include "ImGuizmo/ImGuizmo.h"


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	bool SaveScene(char* file_name);
	bool LoadScene(char* file_name);

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
	vector<ComponentCamera*> cameras;

	void DrawGuizmos();
	ImGuizmo::OPERATION guizmo_operation;

	GameObject* root;
	bool grid;
};
