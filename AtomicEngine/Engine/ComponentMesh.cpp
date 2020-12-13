#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentMaterial.h"
#include "ModuleJSON.h"

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
	
void ComponentMesh::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;

	save_object.AddInt("Type", (int)type);
	//save_object.AddInt("MeshID", _resource->GetUID());
	if (m != nullptr)
	{
		save_object.AddString("Path", m->filename);
		save_object.AddInt("id_index", m->id_index);
		save_object.AddInt("num_index", m->num_index);
		save_object.AddInt("index", *m->index);
		save_object.AddInt("id_vertex", m->id_vertex);
		save_object.AddInt("num_vertex", m->num_vertex);
		save_object.AddInt("vertex", *m->vertex);
		save_object.AddInt("id_normals", m->id_normals);
		save_object.AddInt("num_normals", m->num_normals);
		save_object.AddInt("normals", *m->normals);
		save_object.AddInt("id_colors", m->id_colors);
		save_object.AddInt("num_colors", m->num_colors);
		save_object.AddInt("colors", *m->colors);
		save_object.AddInt("id_texcoords", m->id_texcoords);
		save_object.AddInt("num_texcoords", m->num_texcoords);
		save_object.AddInt("texcoords", *m->texcoords);
		save_object.AddBool("vNormals", m->vNormals);
		save_object.AddBool("fNormals", m->fNormals);
		save_object.AddFloat("r", m->r);
		save_object.AddFloat("g", m->g);
		save_object.AddFloat("b", m->b);
		save_object.AddBool("color", m->color);
	}

	save_array.AddObject(save_object);
}

void ComponentMesh::Load(GnJSONObj& load_object)
{
	//int meshUID = load_object.GetInt("MeshID");
	//SetResourceUID(meshUID);
	if (m == nullptr) {
		m = new Mesh();
	}
	m->filename = (char*)load_object.GetString("Path", "Untitled");
	m->id_index = load_object.GetInt("id_index");
	m->num_index = load_object.GetInt("num_index");
	*m->index = load_object.GetInt("index");
	m->id_vertex = load_object.GetInt("id_vertex");
	m->num_vertex = load_object.GetInt("num_vertex");
	*m->vertex = load_object.GetInt("vertex");
	m->id_normals = load_object.GetInt("id_normals");
	m->num_normals = load_object.GetInt("num_normals");
	*m->normals = load_object.GetInt("normals");
	m->id_colors = load_object.GetInt("id_colors");
	m->num_colors = load_object.GetInt("num_colors");
	*m->colors = load_object.GetInt("colors");
	m->id_texcoords = load_object.GetInt("id_texcoords");
	m->num_texcoords = load_object.GetInt("num_texcoords");
	*m->texcoords = load_object.GetInt("texcoords");
	m->vNormals = load_object.GetBool("vNormals");
	m->fNormals = load_object.GetBool("fNormals");
	m->r = load_object.GetFloat("r");
	m->g = load_object.GetFloat("g");
	m->b = load_object.GetFloat("b");
	m->color = load_object.GetBool("color");
}