#include "ComponentCamera.h"

ComponentCamera::ComponentCamera() : Component()
{
	type = ComponentType::Camera;

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 1000.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	UpdatePlanes();
	update_projection = true;
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
}

void ComponentCamera::Disable()
{
	active = false;
}

float ComponentCamera::GetNearPlane() const
{
	return frustum.NearPlaneDistance();
}

float ComponentCamera::GetFarPlane() const
{
	return frustum.FarPlaneDistance();
}

//Returns FOV in degrees
float ComponentCamera::GetFOV() const
{
	return frustum.VerticalFov() * RADTODEG;
}

float ComponentCamera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

void ComponentCamera::SetNearPlane(float distance)
{
	if (distance > 0 && distance < frustum.FarPlaneDistance())
	{
		frustum.SetViewPlaneDistances(distance, frustum.FarPlaneDistance());
	}
	UpdatePlanes();
	update_projection = true;
}

void ComponentCamera::SetFarPlane(float distance)
{
	if (distance > 0 && distance > frustum.NearPlaneDistance())
	{
		frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), distance);
	}
	UpdatePlanes();
	update_projection = true;
}

//Setting vertical FOV in degrees 
void ComponentCamera::SetFOV(float fov)
{
	fov *= DEGTORAD;
	frustum.SetVerticalFovAndAspectRatio(fov, frustum.AspectRatio());
	UpdatePlanes();
	update_projection = true;
}

void ComponentCamera::SetAspectRatio(float ar)
{
	float horizontalFov = frustum.HorizontalFov();
	frustum.SetHorizontalFovAndAspectRatio(horizontalFov, ar);
	UpdatePlanes();
	update_projection = true;
}
//--------------------------------

void ComponentCamera::Look(const float3& position)
{
	float3 vector = position - frustum.Pos();

	float3x3 matrix = float3x3::LookAt(frustum.Front(), vector.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(matrix.MulDir(frustum.Up()).Normalized());
	UpdatePlanes();
	update_projection = true;
}

void ComponentCamera::Match(const C_Camera* reference)
{
	frustum.SetPos(reference->frustum.Pos());
	frustum.SetFront(reference->frustum.Front());
	frustum.SetUp(reference->frustum.Up());
	UpdatePlanes();
	update_projection = true;
}

float* ComponentCamera::GetOpenGLViewMatrix()
{
	static float4x4 m;

	m = frustum.ViewMatrix();

	m.Transpose();

	return (float*)m.v;
}

float* ComponentCamera::GetOpenGLProjectionMatrix()
{
	static float4x4 m;

	m = frustum.ProjectionMatrix();
	m.Transpose();

	return (float*)m.v;
}

void ComponentCamera::OnUpdateTransform(const float4x4& global, const float4x4& parent_global)
{
	frustum.SetFront(global.WorldZ());
	frustum.SetUp(global.WorldY());

	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat quat = Quat::identity;
	global.Decompose(position, quat, scale);

	frustum.SetPos(position);
	UpdatePlanes();

	update_projection = true;
}

/*void ComponentCamera::Save()
{

}

void ComponentCamera::Load()
{

}*/

void ComponentCamera::UpdatePlanes()
{
	frustum.GetPlanes(planes);
}
