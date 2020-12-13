#include "Application.h"
#include "AtTime.h"
#include "Globals.h"
#include "ModuleJSON.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	gui = new ModuleGUI(this);
	importer = new ModuleImporter(this);
	fileSystem = new ModuleFileSystem(this);

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(importer);
	AddModule(fileSystem);

	// Scenes
	AddModule(scene_intro);
	AddModule(gui);

	AddModule(renderer3D);

	want_to_load = false;
	want_to_save = false;
	fps = 0;
	ms_cap = 1000 / 120;
	inGame = false;
	frames_since_start = 0;
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
	atLOG("Application Start --------------");
	for (int i = 0; i < list_modules.size() && ret == true; i++)
	{
		ret = list_modules[i]->Start();
	}

	Time::RealTime.Start();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
	fps = 1.0f / dt;

	Time::RealTime.Step();
	Time::GameTime.Step();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	int last_frame_ms = ms_timer.Read();

	if (last_frame_ms < ms_cap)
	{
		SDL_Delay(ms_cap - last_frame_ms);
	}

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

	if (want_to_save)
	{
		scene_intro->SaveScene(file_to_save);
		want_to_save = false;
	}

	if (want_to_load)
	{
		scene_intro->LoadScene(file_to_load);
		want_to_load = false;
	}

	frames_since_start++;
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

const char* Application::GetTitleName() const
{
	return title.c_str();
}

const char* Application::GetOrganizationName() const
{
	return organization.c_str();
}

void Application::StartGame()
{
	if (inGame == false) 
	{
		inGame = true;
		Time::GameTime.Start();
	}
}

void Application::StopGame()
{
	inGame = false;
	Time::GameTime.Stop();
}

void Application::Save(const char* file_name)
{
	want_to_save = true;
	strcpy_s(file_to_save, file_name);
}

void Application::Load(const char* file_name)
{
	want_to_load = true;
	strcpy_s(file_to_load, file_name);
}