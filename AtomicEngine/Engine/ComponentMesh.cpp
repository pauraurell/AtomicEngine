#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentMaterial.h"

ComponentMesh::ComponentMesh(GameObject* go) : Component()
{
	type = ComponentType::Mesh;
	owner = go;
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

}

void ComponentMesh::Disable()
{
	active = false;
}
	
