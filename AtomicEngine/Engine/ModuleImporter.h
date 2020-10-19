#pragma once
#include "Globals.h"
#include "Module.h"

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	update_status Update();
	bool CleanUp();

	void LoadMesh(char* file_path);
	
	struct mesh
	{
		uint id_index = 0; // index in VRAM
		uint num_index = 0;
		uint* index = nullptr;

		uint id_vertex = 0; // unique vertex in VRAM
		uint num_vertex = 0;
		float* vertex = nullptr;
	};
	mesh myMesh;


};