#pragma once
#include "Globals.h"
#include "Module.h"
#include "Mesh.h"
#include "Texture.h"
#include <vector>
#include <string>
#include "GameObject.h"

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

#include "ComponentMesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

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

	Mesh* LoadMesh(const aiScene* scene, aiNode* node, const char* path);
	GameObject* PreorderChildren(const aiScene* scene, aiNode* node, aiNode* parentNode, GameObject* parentGameObject, char* path);
	void LoadTransform(aiNode* node, ComponentTransform* transform);

	GLuint Gl_Tex;
};