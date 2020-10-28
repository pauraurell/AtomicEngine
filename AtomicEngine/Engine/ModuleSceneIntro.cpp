#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "Component.h"
#include "ComponentMesh.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Start()
{
	bool ret = true;
	grid = true;
	App->camera->Move(vec3(-3, 2, 1));
	App->camera->LookAt(vec3(0.f, 0.f, 0.f));

	App->importer->LoadMesh("BakerHouse.fbx");
	return ret;
}

bool ModuleSceneIntro::CleanUp()
{
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	pPlane plane(0.f, 1.f, 0.f, 1.f);
	plane.axis = true;

	if (grid)
		plane.Render(App->gui->gridSize);

	if (game_objects.size() > 0) 
	{
		for (int i = 0; i < game_objects.size(); ++i)
		{
			game_objects[i]->Update();
		}
	}

	return UPDATE_CONTINUE;
}

GameObject* ModuleSceneIntro::CreateGameObject()
{
	GameObject* newGameObject = new GameObject();
	newGameObject->CreateComponent(ComponentType::Transform);
	game_objects.push_back(newGameObject);
	return newGameObject;
}

GameObject* ModuleSceneIntro::CreateGameObject(mesh* mesh)
{
	GameObject* newGameObject = new GameObject;
	newGameObject->CreateComponent(ComponentType::Transform);
	newGameObject->CreateComponent(ComponentType::Mesh);
	newGameObject->GetCMesh()->m = mesh;
	App->scene_intro->game_objects.push_back(newGameObject);
	return newGameObject;
}

void ModuleSceneIntro::DeleteGameObject(GameObject* to_delete)
{
	for (int i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i] == to_delete)
		{
			App->gui->printInspector = false;
			game_objects[i]->DeleteComponents();
			game_objects.erase(game_objects.begin() + i);
			delete to_delete;
			break;
		}
	}

}

