#pragma once
#include "Globals.h"
#include "Module.h"
#include "Mesh.h"

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	update_status Update();
	bool CleanUp();

	void LoadMesh(char* file_path);
	void LoadTexture(char* file_path);
		
	mesh myMesh;
};