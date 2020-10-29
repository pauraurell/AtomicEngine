#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "Mesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleImporter::~ModuleImporter()
{

}

bool ModuleImporter::Init() {
	
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	
	return true;
}

update_status ModuleImporter::Update() {

	return UPDATE_CONTINUE;
}


bool ModuleImporter::CleanUp() {

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void ModuleImporter::LoadMesh(char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	myMesh.filename = file_path;
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* ourMesh = scene->mMeshes[i];

			myMesh.num_vertex = ourMesh->mNumVertices;
			myMesh.vertex = new float[myMesh.num_vertex * 3];
			memcpy(myMesh.vertex, ourMesh->mVertices, sizeof(float) * myMesh.num_vertex * 3);
			LOG("New mesh with %d vertices", myMesh.num_vertex);

			// copy faces
			if (ourMesh->HasFaces())
			{
				myMesh.num_index = ourMesh->mNumFaces * 3;
				myMesh.index = new uint[myMesh.num_index]; // assume each face is a triangle
				for (uint i = 0; i < ourMesh->mNumFaces; ++i)
				{
					if (ourMesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&myMesh.index[i * 3], ourMesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

			if (ourMesh->HasNormals())
			{
				myMesh.num_normals = ourMesh->mNumVertices;
				myMesh.normals = new float[myMesh.num_normals * 3];

				for (unsigned int i = 0, v = 0; i < myMesh.num_normals; i++, v += 3)
				{
					myMesh.normals[v] = ourMesh->mNormals[i].x;
					myMesh.normals[v + 1] = ourMesh->mNormals[i].y;
					myMesh.normals[v + 2] = ourMesh->mNormals[i].z;
				}
			}

			if (ourMesh->HasVertexColors(0))
			{
				myMesh.num_colors = ourMesh->mNumVertices;
				myMesh.colors = new float[myMesh.num_colors * 4];

				for (unsigned int i = 0, v = 0; i < myMesh.num_colors; i++, v += 4)
				{
					myMesh.colors[v] = ourMesh->mColors[0][i].r;
					myMesh.colors[v + 1] = ourMesh->mColors[0][i].g;
					myMesh.colors[v + 2] = ourMesh->mColors[0][i].b;
					myMesh.colors[v + 3] = ourMesh->mColors[0][i].a;
				}
			}

			if (ourMesh->HasTextureCoords(0))
			{
				myMesh.num_texcoords = ourMesh->mNumVertices;
				myMesh.texcoords = new float[myMesh.num_texcoords * 2];

				for (unsigned int i = 0, v = 0; i < myMesh.num_texcoords; i++, v += 2)
				{
					myMesh.texcoords[v] = ourMesh->mTextureCoords[0][i].x;
					myMesh.texcoords[v + 1] = ourMesh->mTextureCoords[0][i].y;
				}
			}
		}
		App->renderer3D->LoadMeshBuffer();
		App->scene_intro->CreateGameObject(&myMesh);
		
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", file_path);
}

void ModuleImporter::LoadTexture(char* file_path)
{
	
}


