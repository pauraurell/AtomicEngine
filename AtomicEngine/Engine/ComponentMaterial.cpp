#include "ComponentMaterial.h"
#include "ModuleJSON.h"

ComponentMaterial::ComponentMaterial(GameObject* go) : Component()
{
	type = ComponentType::Material;
	owner = go;
	hasTex = false;
}

ComponentMaterial::ComponentMaterial(GameObject* go, At_Tex* text) : Component()
{
	type = ComponentType::Material;
	owner = go;
	tex = text;
	hasTex = true;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Enable()
{
	active = true;
}

void ComponentMaterial::Update()
{
}

void ComponentMaterial::Disable()
{
	active = false;
}

void ComponentMaterial::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;

	save_object.AddInt("Type", (int)type);
	//save_object.AddInt("Material UID", _resource->GetUID());

	save_object.AddBool("hasTex", hasTex);
	if (tex != nullptr)
	{
		save_object.AddString("texName", tex->texName);
		save_object.AddInt("Gl_Tex", tex->Gl_Tex);
		save_object.AddBool("loaded", tex->loaded);
		save_object.AddBool("checkers", tex->checkers);
		save_object.AddBool("visible", tex->visible);
		save_object.AddInt("w", tex->w);
		save_object.AddInt("h", tex->h);
	}

	save_array.AddObject(save_object);
}

void ComponentMaterial::Load(GnJSONObj& load_object)
{
	/*_resourceUID = load_object.GetInt("Material UID", 0);
	_resource = (ResourceMaterial*)App->resources->RequestResource(_resourceUID);

	int textureUID = load_object.GetInt("Texture UID", -1);

	if (_resource != nullptr && textureUID != -1)
	{
		_resource->diffuseTextureUID = textureUID;
		_diffuseTexture = (ResourceTexture*)App->resources->RequestResource(textureUID);
	}

	if (_diffuseTexture == nullptr)
		AssignCheckersImage();*/

	hasTex = load_object.GetBool("hasTex");

	tex = new At_Tex();
	tex->texName = (char*)load_object.GetString("texName", "Untitled");
	tex->Gl_Tex = load_object.GetInt("Gl_Tex");
	tex->loaded = load_object.GetBool("loaded");
	tex->checkers = load_object.GetBool("checkers");
	tex->visible = load_object.GetBool("visible");
	tex->w = load_object.GetInt("w");
	tex->h = load_object.GetInt("h");
}
