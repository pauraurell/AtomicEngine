#include "Application.h"
#include "Time.h"

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
	LOG("Application Start --------------");
	for (int i = 0; i < list_modules.size() && ret == true; i++)
	{
		ret = list_modules[i]->Start();
	}

	Time::RealTime.Start();

	LoadConfigFile();

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

void Application::SetTitleName(const char* _name)
{
	title = _name;
	window->SetTitle(title.c_str());
}

const char* Application::GetTitleName() const
{
	return title.c_str();
}

const char* Application::GetOrganizationName() const
{
	return organization.c_str();
}

void Application::SetEngineVersion(double _version)
{
	engine_version = _version;
}

double Application::GetEngineVersion() const
{
	return engine_version;
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
bool Application::LoadConfigFile()
{
	bool ret = false;

	char* buffer = nullptr;
	uint size = App->fileSystem->readFile("Assets/Config/config.json", &buffer);

	if (size < 0) { return ret; }
		
	ConfigFile conf(buffer);
	RELEASE_ARRAY(buffer);

	ConfigFile data = conf.GetArray("App", 0);
	std::string name = data.GetString("Name", "unknown name");
	SetTitleName(name.c_str());
	double version = data.GetNumber("Version", 0);
	SetEngineVersion(version);

	for (uint i = 0; i < list_modules.size(); i++)
	{
		ConfigFile module = conf.GetArray(list_modules[i]->name.c_str(), 0);
		ret = list_modules[i]->LoadConfig(&module);
	}

	return ret;
}

bool Application::SaveConfigFile() const
{
	bool ret = false;

	ConfigFile save;
	save.AddArray("App");
	ConfigFile data;
	data.AddString("Name", title.c_str());
	data.AddFloat("Version", engine_version);
	save.AddArrayChild(data);

	for (uint i = 0; i < list_modules.size(); i++)
	{
		ConfigFile module;
		save.AddArray(list_modules[i]->name.c_str());

		ret &= list_modules[i]->SaveConfig(&module);
		save.AddArrayChild(module);
	}

	char* buffer = nullptr;
	uint size = save.Save(&buffer);

	App->fileSystem->writeFile("Assets/Config/config.json", buffer, size);

	return ret;
}
