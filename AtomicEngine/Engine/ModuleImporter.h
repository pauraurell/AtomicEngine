#pragma once
#include "Globals.h"
#include "Module.h"

struct mesh
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	uint	id_normals = 0; // unique normals in VRAM
	uint	num_normals = 0;
	float* normals = nullptr;

	uint	id_colors = 0; // unique colors in VRAM
	uint	num_colors = 0;
	float* colors = nullptr;

	uint	id_texcoords = 0; // unique texcoords in VRAM
	uint	num_texcoords = 0;
	float* texcoords = nullptr;
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	update_status Update();
	bool CleanUp();

	void LoadMesh(char* file_path);
		
	mesh myMesh;
};