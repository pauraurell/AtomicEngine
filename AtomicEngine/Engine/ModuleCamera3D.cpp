#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	cam_speed = 0.15;
	sensitivity = 5;
	speed_multiplier = 1.75f;
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

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) {
		
		GoToOrigin();
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
		if (App->input->GetMouseXMotion() < 0) {
			Position += X * mid_button_speed;
		}
		if (App->input->GetMouseYMotion() > 0) {
			Position += Y * mid_button_speed;
		}
		if (App->input->GetMouseXMotion() > 0) {
			Position -= X * mid_button_speed;
		}
		if (App->input->GetMouseYMotion() < 0) {
			Position -= Y * mid_button_speed;
		}
	}
	
	// Mouse motion ----------------
	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		// TODO (Homework): Rotate the camera with the mouse
		vec3 Forward = -Z;

		Forward = rotate(Forward, (float)dx * (float)( 0.05f * (float)sensitivity), Y);
		Forward = rotate(Forward, (float)dy * (float)( 0.05f * (float)sensitivity), X);

		LookAt(Forward + Position);

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			if (App->input->GetMouseXMotion() > 0) {
				Position -= X * speed;
				Reference -= X * speed;
			}

			if (App->input->GetMouseXMotion() < 0) {
				Position += X * speed;
				Reference += X * speed;
			}
			LookAt(vec3(0, 0, 0));
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
	Position = vec3(5.0f, 5.5f, 5.0f);
}
