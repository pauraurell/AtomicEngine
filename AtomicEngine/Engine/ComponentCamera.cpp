#include "ComponentCamera.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ImGui/imgui.h"

ComponentCamera::ComponentCamera(GameObject* go) : Component()
{
	type = ComponentType::Camera;
	owner = go;

	fixedFOV = FIXED_HORIZONTAL_FOV;
	aspectRatio = 16.0f / 9.0f;
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3(0.0f, 0.0f, -5.0f);
	frustum.up = float3(0.0f, 1.0f, 0.0f);
	frustum.front = float3(0.0f, 0.0f, 1.0f);

	frustum.horizontalFov = 60.0f * DEGTORAD;
	AdjustFieldOfView();

	frustum.nearPlaneDistance = 0.3f;
	frustum.farPlaneDistance = 1000.0f;
}

/*ComponentCamera::ComponentCamera() : Component()
{
	type = ComponentType::Camera;
}*/

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Enable()
{
	active = true;
}

void ComponentCamera::Update()
{
	frustum.pos = owner->GetCTransform()->pos;

	/*frustum.up = owner->GetCTransform()->GetGlobalTransform().WorldY();
	frustum.front = owner->GetCTransform()->GetGlobalTransform().WorldZ();*/

	float3 corner_points[8];
	frustum.GetCornerPoints(corner_points);
	App->renderer3D->DrawAABB(corner_points);
}

void ComponentCamera::Disable()
{
	active = false;
}

void ComponentCamera::SetFixedFOV(FixedFOV g_fixedFOV)
{
	fixedFOV = g_fixedFOV;
}

void ComponentCamera::AdjustFieldOfView()
{
	frustum.verticalFov = 2 * atan(tan(frustum.horizontalFov * 0.5f) * (1 / aspectRatio));
	/*
	switch (_aspectRatio)
	{
	case AR_16_9:
		//_frustum.horizontalFov = 2.0f * std::atan(std::tan(_frustum.verticalFov * 0.5f) * (16.0f / 9.0f));
		_frustum.verticalFov = 2 * atan(tan(_frustum.horizontalFov * 0.5f) * (9.0f / 16.0f));
		break;
	case AR_16_10:
		//_frustum.horizontalFov = 2.0f * std::atan(std::tan(_frustum.verticalFov * 0.5f) * (16.0f / 10.0f));
		_frustum.verticalFov = 2 * atan(tan(_frustum.horizontalFov * 0.5f) * (10.0f / 16.0f));
		break;
	case AR_4_3:
		//_frustum.horizontalFov = 2.0f * std::atan(std::tan(_frustum.verticalFov * 0.5f) * (4.0f / 3.0f));
		_frustum.verticalFov = 2 * atan(tan(_frustum.horizontalFov * 0.5f) * (3.0f / 4.0f));
		break;
	default:
		break;
	}
	*/
}

void ComponentCamera::AdjustFieldOfView(float width, float height)
{
	if (fixedFOV == FIXED_HORIZONTAL_FOV)
		frustum.verticalFov = 2.0f * std::atan(std::tan(frustum.horizontalFov * 0.5f) * (height / width));
	else
		frustum.horizontalFov = 2.0f * std::atan(std::tan(frustum.verticalFov * 0.5f) * (width / height));
}

void ComponentCamera::SetVerticalFieldOfView(float verticalFOV, float screen_width, float screen_height)
{
	frustum.verticalFov = verticalFOV;
	frustum.horizontalFov = 2.0f * std::atan(std::tan(frustum.verticalFov * 0.5f) * (screen_width / screen_height));
}

void ComponentCamera::SetHorizontalFieldOfView(float horizontalFOV, float screen_width, float screen_height)
{
	frustum.horizontalFov = horizontalFOV;
	frustum.verticalFov = 2.0f * std::atan(std::tan(frustum.horizontalFov * 0.5f) * (screen_height / screen_width));
}

void ComponentCamera::SetPosition(float3 position)
{
	frustum.pos = position;
}

void ComponentCamera::SetReference(float3 ref)
{
	reference = ref;
}

void ComponentCamera::SetNearPlaneDistance(float distance)
{
	frustum.nearPlaneDistance = distance;
}

void ComponentCamera::SetFarPlaneDistance(float distance)
{
	frustum.farPlaneDistance = distance;
}

void ComponentCamera::Look(float3 spot)
{
	float3 difference = spot - frustum.pos;

	float3x3 matrix = float3x3::LookAt(frustum.front, difference.Normalized(), frustum.up, float3::unitY);

	frustum.front = matrix.MulDir(frustum.front).Normalized();
	frustum.up = matrix.MulDir(frustum.up).Normalized();
}

Frustum ComponentCamera::GetFrustum()
{
	return frustum;
}

float* ComponentCamera::GetViewMatrix()
{
	float4x4 viewMatrix;

	viewMatrix = frustum.ViewMatrix();
	viewMatrix.Transpose();

	return (float*)viewMatrix.v;
}

float* ComponentCamera::GetProjectionMatrix()
{
	float4x4 projectionMatrix;

	projectionMatrix = frustum.ProjectionMatrix();
	projectionMatrix.Transpose();

	return (float*)projectionMatrix.v;
}

bool ComponentCamera::ContainsAABB(AABB& aabb)
{
	float3 cornerPoints[8];
	int totalInside = 0;

	//get frustum planes
	Plane* m_plane = new Plane[6]();
	frustum.GetPlanes(m_plane);

	//get AABB points
	aabb.GetCornerPoints(cornerPoints);

	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;

		for (int i = 0; i < 8; ++i)
		{
			if (m_plane[p].normal.Dot(cornerPoints[i]) - m_plane[p].d >= 0.0f)
			{
				iPtIn = 0;
				--iInCount;
			}
		}

		if (iInCount == 0)
			return false;

		totalInside += iPtIn;
	}

	//Totally inside camera view
	if (totalInside == 6)
		return true;

	//Partially inside camera view
	return true;
}