#pragma once
#include "Globals.h"
#include "Module.h"
#include "Mesh.h"
#include "Texture.h"
#include <vector>
#include <string>

#include "Glew\include\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Devil\Include\il.h"
#include "Devil\Include\ilu.h"
#include "Devil\Include\ilut.h"

#pragma comment (lib, "glu32.lib")   
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "Glew/libx86/glew32.lib") 

#pragma comment (lib, "Devil/lib/ILU.lib") 
#pragma comment (lib, "Devil/lib/ILUT.lib") 
#pragma comment (lib, "Devil/lib/DevIL.lib") 

#pragma comment (lib, "Assimp/libx86/assimp.lib")

using namespace std;

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	void LoadMesh(char* file_path, string name = "none");
	void LoadTexture(char* file_path);
	void LoadCheckerTexture();
		
	void GenerateBuffers(Mesh* m);

	GLuint Gl_Tex;

};