#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ComponentTransform.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "camera";
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(4.0f, 3.5f, -8.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	speed_multiplier = 2.0f;
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update()
{
	float camera_speed_weel = 0.35f;
	float speed = cam_speed;
	// OnKeys WASD keys -----------------------------------

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) 
	{
		if (App->gui->selectedObj != nullptr)
		{
			LookAtSelectedObject(true);
		}
	}
	
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		speed = cam_speed * speed_multiplier;
	}
	else {
		speed = cam_speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		Position -= Z * speed;
		Reference -= Z * speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		Position += Z * speed;
		Reference += Z * speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		Position -= X * speed;
		Reference -= X * speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		Position += X * speed;
		Reference += X * speed;
	}

	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT){
		float mid_button_speed = 0.1f;

		if (App->input->GetMouseXMotion() < 2) {
			Position += X * mid_button_speed;
		}

		if (App->input->GetMouseXMotion() > -2) {
			Position -= X * mid_button_speed;
		}

		if (App->input->GetMouseYMotion() > 2) {
			Position += Y * mid_button_speed*2;
		}

		if (App->input->GetMouseYMotion() < -2) {
			Position -= Y * mid_button_speed;
		}
	}
	
	if (App->input->GetMouseZ() > 0)
	{
		Position -= Z * camera_speed_weel;
	}

	if (App->input->GetMouseZ() < 0)
	{
		Position += Z * camera_speed_weel;
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		if (App->gui->selectedObj != nullptr)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			if (dx != 0) { Z = rotate(Z, (float)dx * (0.1f * sensitivity), Y); }
			if (dy != 0) { Z = rotate(Z, (float)dy * (0.1f * sensitivity), X); }

			Position = Z * length(Position);

			LookAtSelectedObject();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->gui->selectedObj != nullptr)
		{
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
		}
	}

	else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		// TODO (Homework): Rotate the camera with the mouse
		vec3 Forward = -Z;

		Forward = rotate(Forward, (float)dx * (float)(0.05f * (float)sensitivity), Y);
		Forward = rotate(Forward, (float)dy * (float)(0.05f * (float)sensitivity), X);

		LookAt(Forward + Position);

	}
	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

// -----------------------------------------------------------------
void ModuleCamera3D::GoToOrigin()
{
	LookAt(vec3(0.f, 0.f, 0.f));
	Position = vec3(2.0f, 3.8f, -5.0f);
}

void ModuleCamera3D::LookAtSelectedObject(bool go_to_gameObj)
{
	float3 position = App->gui->selectedObj->GetCTransform()->pos;
	LookAt(vec3(position.x, position.y, position.z));
	if (go_to_gameObj) Position = vec3(position.x + 2.0f, position.y + 3.8f, position.z - 5.0f);
}

bool ModuleCamera3D::LoadConfig(ConfigFile* data)
{
	cam_speed = data->GetFloat("cameraSpeed");
	sensitivity = data->GetFloat("mouseSensitivity");
	return true;
}

bool ModuleCamera3D::SaveConfig(ConfigFile* data) const
{
	data->AddFloat("cameraSpeed", cam_speed);
	data->AddFloat("mouseSensitivity", sensitivity);
	return true;
}
