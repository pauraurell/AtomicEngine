#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ModuleJSON.h"
#include <vector>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Start()
{
	bool ret = true;

	root = new GameObject();
	root->name = "Root Node";

	grid = true;
	App->camera->Move(float3(-3, 2, 1));
	App->camera->LookAt(float3(0.f, 0.f, 0.f));

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

update_status ModuleSceneIntro::PreUpdate()
{
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) && ImGuizmo::IsUsing() == false)
	{
		GameObject* obj = App->camera->PickGameObject();
		if (obj != nullptr) 
		{
			App->gui->selectedObj = obj;
			App->gui->printInspector = true;
		}
	}

	if ((App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)) { guizmo_operation = ImGuizmo::OPERATION::TRANSLATE; }
	else if ((App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)) { guizmo_operation = ImGuizmo::OPERATION::ROTATE; }
	else if ((App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)) { guizmo_operation = ImGuizmo::OPERATION::SCALE; }

	return UPDATE_CONTINUE;
}

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
	DrawGuizmos();

	return UPDATE_CONTINUE;
}

GameObject* ModuleSceneIntro::CreateGameObject(bool camera)
{
	GameObject* newGameObject = new GameObject();

	newGameObject->SetRootChild();

	newGameObject->CreateComponent(ComponentType::Transform);
	game_objects.push_back(newGameObject);

	if (camera == true)
	{
		newGameObject->CreateComponent(ComponentType::Camera);
		newGameObject->name = "Camera";
	}

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

GameObject* ModuleSceneIntro::CreateGameObject(Mesh* mesh, string name, float3 pos, Quat rot, float3 scale)
{
	GameObject* newGameObject = new GameObject(name.c_str());

	newGameObject->SetRootChild();

	newGameObject->CreateComponent(ComponentType::Transform);
	newGameObject->CreateComponent(ComponentType::Mesh);
	newGameObject->GetCMesh()->m = mesh;
	App->scene_intro->game_objects.push_back(newGameObject);

	newGameObject->GetCTransform()->SetPosition(pos.x, pos.y, pos.z);
	newGameObject->GetCTransform()->SetRotation(rot);
	newGameObject->GetCTransform()->SetScale(scale.x, scale.y, scale.z);


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

void ModuleSceneIntro::DrawGuizmos()
{
	if (App->gui->selectedObj != nullptr) 
	{
		float4x4 viewMatrix = App->camera->GetViewMatrixM().Transposed();
		float4x4 projectionMatrix = App->camera->GetProjectionMatrixM().Transposed();
		float4x4 objectTransform = App->gui->selectedObj->GetCTransform()->globalMat.Transposed();

		bool open = true;
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove;
		ImGui::Begin("Debug", &open, flags);
		ImGuizmo::SetDrawlist();
		ImGui::End();
		ImGuizmo::SetRect(0, 0, App->gui->width, App->gui->height);

		float tempTransform[16];
		memcpy(tempTransform, objectTransform.ptr(), 16 * sizeof(float));
		ImGuizmo::Manipulate(viewMatrix.ptr(), projectionMatrix.ptr(), guizmo_operation, ImGuizmo::MODE::WORLD, tempTransform);

		if (ImGuizmo::IsUsing()) { App->gui->selectedObj->GetCTransform()->SetTransform(tempTransform); }
	}	
}

bool ModuleSceneIntro::SaveScene(char* file_name)
{
	bool ret = true;

	GnJSONObj save_file;

	GnJSONArray gameObjects = save_file.AddArray("Game Objects");

	root->Save(gameObjects);

	char* buffer = NULL;
	uint size = save_file.Save(&buffer);

	App->fileSystem->Save(file_name, buffer, size);

	std::string assets_path = "Assets/Scenes/";
	assets_path.append(App->fileSystem->GetFile(file_name));
	App->fileSystem->DuplicateFile(file_name, assets_path.c_str());

	save_file.Release();
	RELEASE_ARRAY(buffer);

	return ret;
}

bool ModuleSceneIntro::LoadScene(char* file_name)
{
	bool ret = true;

	std::string format = App->fileSystem->GetFileFormat(file_name);
	if (format != ".scene")
	{
		//LOG("%s is not a valid scene format and can't be loaded", file_name);
		return false;
	}

	//ClearScene();

	char* buffer = NULL;
	App->fileSystem->Load(file_name, &buffer);

	GnJSONObj base_object(buffer);
	GnJSONArray gameObjects(base_object.GetArray("Game Objects"));

	std::vector<GameObject*> createdObjects;

	for (size_t i = 0; i < gameObjects.Size(); i++)
	{
		//load game object
		GameObject* gameObject = new GameObject();
		uint parentUUID = gameObject->Load(&gameObjects.GetObjectAt(i));
		createdObjects.push_back(gameObject);

		//check if it's the root game object
		if (strcmp(gameObject->name.c_str(), "Root") == 0) {
			root = gameObject;
			App->gui->selectedObj = root;
		}

		//Get game object's parent
		for (size_t i = 0; i < createdObjects.size(); i++)
		{
			if (createdObjects[i]->UUID == parentUUID)
			{
				createdObjects[i]->CreateChild(gameObject);
			}
		}
	}

	root->ChildrenTransform();

	if (root != nullptr)
		//LOG("Scene: %s loaded successfully", file_name);

	return ret;
}