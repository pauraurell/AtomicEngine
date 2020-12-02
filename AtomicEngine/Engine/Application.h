#pragma once
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "ModuleGUI.h"
#include "ModuleImporter.h"
#include "ModuleFileSystem.h"
#include <vector>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleGUI* gui;
	ModuleImporter* importer;
	ModuleFileSystem* fileSystem;


private:

		std::vector<Module*> list_modules;
		Timer	ms_timer;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();


private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:

	void StartGame();
	void StopGame();

public:

	bool inGame;
	float	ms_cap;
	float	dt;
	float	fps;
	int frames_since_start;

	std::string title;
	std::string organization;

	const char* GetTitleName() const;
	const char* GetOrganizationName() const;
};

extern Application* App;