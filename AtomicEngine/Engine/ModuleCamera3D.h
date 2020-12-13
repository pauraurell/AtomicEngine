#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/MathGeoLib.h"

//class GameObject;
class ComponentCamera;
enum FixedFOV;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnResize(int width, int height);
	void Look(float3& position);
	void LookAt(const float3& Spot);
	void LookAt(const vec3& Spot);
	ComponentCamera* GetCamera();
	float* GetViewMatrix();
	float4x4 GetViewMatrixM();
	float* GetProjectionMatrix();
	float4x4 GetProjectionMatrixM();
	float3 GetPosition();
	
	void GoToOrigin();
	void LookAtSelectedObject(bool go_to_gameObj = false);

	FixedFOV GetFixedFOV();
	void SetFixedFOV(FixedFOV fixedFOV);
	float GetVerticalFieldOfView();
	float GetHorizontalFieldOfView();
	void SetVerticalFieldOfView(float verticalFOV, int screen_width, int screen_height);
	void SetHorizontalFieldOfView(float horizontalFOV, int screen_width, int screen_height);
	void Reset();

	GameObject* PickGameObject();
	LineSegment CreateRay();
	Triangle CreateTri(Mesh*m, int i);

	void Move(const float3 &Movement);
	void Orbit();
	

private:

	//void CalculateViewMatrix();

public:
	
	float3 X, Y, Z, Position, Reference;
	int sensitivity;
	float cam_speed;
	float speed_multiplier;
	ComponentCamera* camera;

private:

	//mat4x4 ViewMatrix, ViewMatrixInverse;
};