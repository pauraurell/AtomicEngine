#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include <map>

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(4.0f, 3.5f, -8.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	cam_speed = 0.15;
	sensitivity = 5;
	speed_multiplier = 2.0f;

	camera = new ComponentCamera();
	camera->SetPosition(float3(Position));
	camera->SetReference(Reference);
	App->scene_intro->cameras.push_back(camera);
	camera->Disable();
	Look(Reference);

	//background = { 0.12f, 0.12f, 0.12f, 1.0f };
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	atLOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	atLOG("Cleaning camera");
	delete camera;
	camera = nullptr;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update()
{
	float3 newPos = float3::zero;
	speed_multiplier = 1;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) { speed_multiplier = 2; }

	//Up/Down
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_REPEAT) { newPos.y += cam_speed; }

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT) { newPos.y -= cam_speed; }

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (App->gui->selectedObj != nullptr)
		{
			float3 center(App->gui->selectedObj->GetCTransform()->pos);
			LookAt(center);
		}
	}

	//Forwards/Backwards
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		newPos += camera->GetFrustum().front * cam_speed * speed_multiplier;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		newPos -= camera->GetFrustum().front * cam_speed * speed_multiplier;
	}
	//Left/Right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		newPos += camera->GetFrustum().WorldRight() * cam_speed * speed_multiplier;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		newPos -= camera->GetFrustum().WorldRight() * cam_speed * speed_multiplier;
	}

	//Drag
	if ((App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT))
	{
		newPos -= camera->GetFrustum().WorldRight() * App->input->GetMouseXMotion() * cam_speed;
		newPos += camera->GetFrustum().up * App->input->GetMouseYMotion() * cam_speed;
	}

	// Zoom 
	if (App->input->GetMouseZ() > 0)
	{
		newPos += camera->GetFrustum().front * cam_speed;
	}
	else if (App->input->GetMouseZ() < 0)
	{
		newPos -= camera->GetFrustum().front * cam_speed;
	}

	if ((App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) || ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)))
	{
		Orbit();
	}

	Position += newPos;
	camera->SetPosition(Position);
	Reference += newPos;
	camera->SetReference(Reference);

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::OnResize(int width, int height)
{
	camera->AdjustFieldOfView(width, height);
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(float3& position)
{
	camera->Look(position);
	Reference = position;
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3& Spot)
{
	camera->Look(Spot);
	Reference = Spot;
}

ComponentCamera* ModuleCamera3D::GetCamera()
{
	return camera;
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return camera->GetViewMatrix();
}

// -----------------------------------------------------------------
float4x4 ModuleCamera3D::GetViewMatrixM()
{
	return camera->GetFrustum().ViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetProjectionMatrix()
{
	return camera->GetProjectionMatrix();
}

float4x4 ModuleCamera3D::GetProjectionMatrixM()
{
	return camera->GetFrustum().ProjectionMatrix();
}

float3 ModuleCamera3D::GetPosition()
{
	return Position;
}

GameObject* ModuleCamera3D::PickGameObject()
{
	float normalized_x = (float)App->input->GetMouseX() / (float)SCREEN_WIDTH;
	float normalized_y = (float)App->input->GetMouseY() / (float)SCREEN_HEIGHT;

	normalized_x = (normalized_x - 0.5f) * 2.0f;
	normalized_y = -(normalized_y - 0.5f) * 2.0f;

	LineSegment my_ray = camera->GetFrustum().UnProjectLineSegment(normalized_x, normalized_y);

	App->renderer3D->ray = my_ray;

	std::vector<GameObject*> sceneGameObjects = App->scene_intro->GetGameObjects();
	std::map<float, GameObject*> hitGameObjects;

	//find all hit GameObjects
	for (size_t i = 0; i < sceneGameObjects.size(); i++)
	{
		bool hit = my_ray.Intersects(sceneGameObjects[i]->BB);

		if (hit)
		{
			float dNear;
			float dFar;
			hit = my_ray.Intersects(sceneGameObjects[i]->BB, dNear, dFar);
			hitGameObjects[dNear] = sceneGameObjects[i];
		}
	}

	std::map<float, GameObject*>::iterator it = hitGameObjects.begin();
	for (it; it != hitGameObjects.end(); it++)
	{
		GameObject* gameObject = it->second;

		LineSegment ray_local_space = my_ray;
		ray_local_space.Transform(gameObject->GetCTransform()->globalMat.Inverted());

		if (gameObject->GetCMesh() != nullptr) 
		{
			Mesh* mesh = (Mesh*)gameObject->GetCMesh()->m;

			for (size_t i = 0; i < mesh->num_index; i += 3)
			{
				//create every triangle
				float3 v1(mesh->vertex[mesh->index[i] * 3], mesh->vertex[mesh->index[i] * 3 + 1],
					mesh->vertex[mesh->index[i] * 3 + 2]);

				float3 v2(mesh->vertex[mesh->index[i + 1] * 3], mesh->vertex[mesh->index[i + 1] * 3 + 1],
					mesh->vertex[mesh->index[i + 1] * 3 + 2]);

				float3 v3(mesh->vertex[mesh->index[i + 2] * 3], mesh->vertex[mesh->index[i + 2] * 3 + 1],
					mesh->vertex[mesh->index[i + 2] * 3 + 2]);

				const Triangle triangle(v1, v2, v3);

				float distance;
				float3 intersectionPoint;
				if (ray_local_space.Intersects(triangle, &distance, &intersectionPoint)) {
					return gameObject;
				}
			}
		}
	}

	return nullptr;
}

FixedFOV ModuleCamera3D::GetFixedFOV()
{
	return camera->fixedFOV;
}

void ModuleCamera3D::SetFixedFOV(FixedFOV fixedFOV)
{
	camera->SetFixedFOV(fixedFOV);
}

float ModuleCamera3D::GetVerticalFieldOfView()
{
	return camera->GetFrustum().verticalFov;
}

float ModuleCamera3D::GetHorizontalFieldOfView()
{
	return camera->GetFrustum().horizontalFov;
}


void ModuleCamera3D::SetVerticalFieldOfView(float verticalFOV, int screen_width, int screen_height)
{
	camera->SetVerticalFieldOfView(verticalFOV, screen_width, screen_height);
	App->renderer3D->UpdateProjMat(camera->GetProjectionMatrix());
}

void ModuleCamera3D::SetHorizontalFieldOfView(float horizontalFOV, int screen_width, int screen_height)
{
	camera->SetHorizontalFieldOfView(horizontalFOV, screen_width, screen_height);
	App->renderer3D->UpdateProjMat(camera->GetProjectionMatrix());
}

void ModuleCamera3D::Reset()
{
	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(4.0f, 3.5f, -8.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	Move(float3(1.0f, 1.0f, 0.0f));
	LookAt(float3(0.0f, 0.0f, 0.0f));
}

void ModuleCamera3D::Move(const float3 &Movement)
{
	Position += Movement;
	Reference += Movement;
}

void ModuleCamera3D::Orbit()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	Quat y_rotation(camera->GetFrustum().up, dx * cam_speed * 0.1f);
	Quat x_rotation(camera->GetFrustum().WorldRight(), dy * cam_speed * 0.1f);

	float3 distance = Position - Reference;
	distance = x_rotation.Transform(distance);
	distance = y_rotation.Transform(distance);

	Position = distance + Reference;
	camera->SetPosition(Position);
	camera->Look(Reference);
}