#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	gui = new ModuleGUI(this);

	// Main Modules
	AddModule(window);
	AddModule(renderer3D);
	AddModule(camera);
	AddModule(input);
	AddModule(gui);
	
	// Scenes
	AddModule(scene_intro);
}

Application::~Application()
{
	std::vector<Module*>::reverse_iterator item = list_modules.rbegin();

	while (item != list_modules.rend())
	{
		delete* item;
		item++;
	}
}

bool Application::Init()
{
	bool ret = true;
	// Call Init() in all modules
	for (int i = 0; i < list_modules.size() && ret == true; i++)
	{
		ret = list_modules[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (int i = 0; i < list_modules.size() && ret == true; i++)
	{
		ret = list_modules[i]->Start();
	}

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{

}

// ---------------------------------------------
void Application::FinishUpdate()
{

}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::vector<Module*>::reverse_iterator item = list_modules.rbegin();

	for (int i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PreUpdate();
	}

	for (int i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->Update();
	}

	for (int i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PostUpdate();
	}

	
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (size_t i = 0; i < list_modules.size() && ret == true; i++)
	{
		ret = list_modules[i]->CleanUp();
	}


	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}