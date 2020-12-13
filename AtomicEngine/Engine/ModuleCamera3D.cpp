#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include <map>

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (App->gui->selectedObj != nullptr)
		{
			LookAtSelectedObject(true);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) 
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) { newPos += camera->GetFrustum().front * cam_speed * speed_multiplier; }
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) { newPos -= camera->GetFrustum().front * cam_speed * speed_multiplier; }
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { newPos += camera->GetFrustum().WorldRight() * cam_speed * speed_multiplier; }
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { newPos -= camera->GetFrustum().WorldRight() * cam_speed * speed_multiplier; }
	}
	else 
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) { newPos += camera->GetFrustum().front * cam_speed; }
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) { newPos -= camera->GetFrustum().front * cam_speed; }
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { newPos += camera->GetFrustum().WorldRight() * cam_speed; }
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { newPos -= camera->GetFrustum().WorldRight() * cam_speed; }
	}

	if ((App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT))
	{
		newPos -= camera->GetFrustum().WorldRight() * App->input->GetMouseXMotion() * cam_speed;
		newPos += camera->GetFrustum().up * App->input->GetMouseYMotion() * cam_speed;
	}

	if (App->input->GetMouseZ() > 0) { newPos += camera->GetFrustum().front * cam_speed; }
	else if (App->input->GetMouseZ() < 0) {	newPos -= camera->GetFrustum().front * cam_speed; }

	if ((App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)){ Orbit(); }

	if ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT))
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			if (App->gui->selectedObj != nullptr)
			{
				Orbit();
				LookAtSelectedObject();
			}
		}

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			/*if (App->gui->selectedObj != nullptr)
			{
				LookAtSelectedObject();
				float zoom_speed = 0.5;

				if (App->input->GetMouseYMotion() > 0)
				{
					Position += Z * zoom_speed;
				}

				if (App->input->GetMouseYMotion() < 0)
				{
					Position -= Z * zoom_speed;
				}

				LookAtSelectedObject();
			}*/
		}
	}
		
	Position += newPos;
	camera->SetPosition(Position);
	Reference += newPos;
	camera->SetReference(Reference);

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::OnResize(int width, int height)
{
	camera->AdjustFieldOfView(width, height);
}

void ModuleCamera3D::Look(float3& position)
{
	camera->Look(position);
	Reference = position;
}

void ModuleCamera3D::LookAt(const float3& Spot)
{
	camera->Look(Spot);
	Reference = Spot;
}

ComponentCamera* ModuleCamera3D::GetCamera()
{
	return camera;
}

float* ModuleCamera3D::GetViewMatrix()
{
	return camera->GetViewMatrix();
}

float4x4 ModuleCamera3D::GetViewMatrixM()
{
	return camera->GetFrustum().ViewMatrix();
}

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
	LineSegment ray = CreateRay();

	std::vector<GameObject*> sceneGameObjects = App->scene_intro->GetGameObjects();
	std::map<float, GameObject*> go;
	for (size_t i = 0; i < sceneGameObjects.size(); i++)
	{
		bool hit = ray.Intersects(sceneGameObjects[i]->BB);
		if (hit) {
			float sDistance;
			float fDistance;
			hit = ray.Intersects(sceneGameObjects[i]->BB, sDistance, fDistance);
			go[sDistance] = sceneGameObjects[i];
		}
	}

	std::map<float, GameObject*>::iterator it = go.begin();
	for (it; it != go.end(); it++)
	{
		GameObject* gameObject = it->second;
		ray.Transform(gameObject->GetCTransform()->globalMat.Inverted());

		if (gameObject->GetCMesh() != nullptr) 
		{
			Mesh* mesh = (Mesh*)gameObject->GetCMesh()->m;

			for (size_t i = 0; i < mesh->num_index; i += 3)
			{
				Triangle tri = CreateTri(mesh, i);
				float d;
				float3 intersectionPoint;
				if (ray.Intersects(tri, &d, &intersectionPoint)) {
					return gameObject;
				}
			}
		}
	}

	return nullptr;
}

LineSegment ModuleCamera3D::CreateRay() 
{
	float x = (((float)App->input->GetMouseX() / (float)App->gui->width ) - 0.50f) * 2.0f;
	float y = -(((float)App->input->GetMouseY() / (float)App->gui->height) - 0.50f) * 2.0f;

	LineSegment line = camera->GetFrustum().UnProjectLineSegment(x, y);
	App->renderer3D->ray = line;

	return line;
}

Triangle ModuleCamera3D::CreateTri(Mesh* m, int i)
{
	float3 vertex1(m->vertex[m->index[i] * 3], m->vertex[m->index[i] * 3 + 1],
		m->vertex[m->index[i] * 3 + 2]);
	float3 vertex2(m->vertex[m->index[i + 1] * 3], m->vertex[m->index[i + 1] * 3 + 1],
		m->vertex[m->index[i + 1] * 3 + 2]);
	float3 vertex3(m->vertex[m->index[i + 2] * 3], m->vertex[m->index[i + 2] * 3 + 1],
		m->vertex[m->index[i + 2] * 3 + 2]);

	const Triangle tri(vertex1, vertex2, vertex3);
	return tri;
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

void ModuleCamera3D::GoToOrigin()
{
	LookAt(float3(0.f, 0.f, 0.f));
	Position = float3(2.0f, 3.8f, -5.0f);
}

void ModuleCamera3D::LookAtSelectedObject(bool go_to_gameObj)
{
	float3 position = App->gui->selectedObj->GetCTransform()->pos;
	LookAt(float3(position.x, position.y, position.z));
	if (go_to_gameObj) Position = float3(position.x + 2.0f, position.y + 3.8f, position.z - 5.0f);
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
	Quat y(camera->GetFrustum().up, -App->input->GetMouseXMotion() * sensitivity * 0.003f);
	Quat x(camera->GetFrustum().WorldRight(), -App->input->GetMouseYMotion() * sensitivity * 0.003f);

	float3 d = Position - Reference;
	d = x.Transform(d);
	d = y.Transform(d);
	Position = d + Reference;
	camera->SetPosition(Position);
	camera->Look(Reference);
}