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

}

void ComponentMesh::Disable()
{
	active = false;
}
	
