#pragma once
#include "Globals.h"
#include "Module.h"
#include "Mesh.h"
#include <vector>
#include <string>

using namespace std;

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	update_status Update();
	bool CleanUp();

	void LoadMesh(char* file_path, string name = "none");
	void LoadTexture(char* file_path);
	void LoadCheckerTexture(char* file_path);
		
	void GenerateBuffers(mesh* m);

	mesh myMesh;
	vector<mesh*> meshes;
};