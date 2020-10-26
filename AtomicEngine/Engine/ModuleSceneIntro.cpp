#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Init()
{
	bool ret = true;
	grid = true;
	App->camera->Move(vec3(-3, 2, 1));
	App->camera->LookAt(vec3(0.f, 0.f, 0.f));

	//App->importer->LoadMesh("BakerHouse.fbx");

	return ret;
}

bool ModuleSceneIntro::CleanUp()
{
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	Plane plane(0.f, 1.f, 0.f, 1.f);
	plane.axis = true;
	Color plane_color(App->gui->gridColor, App->gui->gridColor, App->gui->gridColor, 1.0f);
	plane.color = plane_color;

	if (grid)
		plane.Render();

	return UPDATE_CONTINUE;
}

