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

	ImGuizmo::Enable(false);

	root = new GameObject();
	root->name = "Root Node";

	grid = true;
	App->camera->Move(vec3(-3, 2, 1));
	App->camera->LookAt(vec3(0.f, 0.f, 0.f));

	App->importer->LoadMesh("Assets/3D Objects/BakerHouse.fbx");

	if (game_objects[0] != nullptr && game_objects[1] != nullptr)
	{
		App->gui->selectedObj = game_objects[0];
		App->importer->LoadTexture("Assets/Textures/BakerHouseTexture.png");
		App->gui->selectedObj = game_objects[1];
		App->importer->LoadTexture("Assets/Textures/BakerHouseTexture.png");
		App->gui->selectedObj = nullptr;
	}

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

	newGameObject->SetRootChild();

	newGameObject->CreateComponent(ComponentType::Transform);
	game_objects.push_back(newGameObject);

	return newGameObject;
}

GameObject* ModuleSceneIntro::CreateGameObject(Mesh* mesh)
{
	GameObject* newGameObject = new GameObject();

	newGameObject->SetRootChild();

	newGameObject->CreateComponent(ComponentType::Transform);
	newGameObject->CreateComponent(ComponentType::Mesh);
	newGameObject->GetCMesh()->m = mesh;
	App->scene_intro->game_objects.push_back(newGameObject);

	return newGameObject;
}

GameObject* ModuleSceneIntro::CreateGameObject(Mesh* mesh, string name)
{
	GameObject* newGameObject = new GameObject(name.c_str());

	newGameObject->SetRootChild();

	newGameObject->CreateComponent(ComponentType::Transform);
	newGameObject->CreateComponent(ComponentType::Mesh);
	newGameObject->GetCMesh()->m = mesh;
	App->scene_intro->game_objects.push_back(newGameObject);

	return newGameObject;
}

void ModuleSceneIntro::DeleteGameObject(GameObject* to_delete)
{
	if (root->DeleteChild(to_delete)) { to_delete->DeleteChildren(); }
	
	else if (to_delete->parent->DeleteChild(to_delete)) { to_delete->DeleteChildren(); }
	
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
	App->gui->selectedObj = nullptr;

}

std::vector<GameObject*> ModuleSceneIntro::GetGameObjects()
{
	vector<GameObject*> temp;
	PushbackGameObjects(root, temp);
	return temp;
}

void ModuleSceneIntro::PushbackGameObjects(GameObject* gameObject, std::vector<GameObject*>& gameObjects)
{
	gameObjects.push_back(gameObject);
	for (size_t i = 0; i < gameObject->children.size(); i++) {
		PushbackGameObjects(gameObject->children[i], gameObjects);
	}
}

