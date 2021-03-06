#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentMaterial.h"

ComponentMesh::ComponentMesh() : Component()
{
	type = ComponentType::Mesh;
}

ComponentMesh::~ComponentMesh()
{
	delete m;
}

void ComponentMesh::Enable()
{
	active = true;
}

void ComponentMesh::Update()
{
	/*if (m != nullptr)
	{
		App->renderer3D->RenderGameObject(m);
	}*/
}

void ComponentMesh::Disable()
{
	active = false;
}
	
