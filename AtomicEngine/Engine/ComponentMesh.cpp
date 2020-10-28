#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentMaterial.h"

ComponentMesh::ComponentMesh(GameObject* c_owner) : Component()
{
	type = ComponentType::Mesh;
	owner = c_owner;
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
	//App->renderer3D->RenderMesh(m);
}

void ComponentMesh::Disable()
{
	active = false;
}


	
