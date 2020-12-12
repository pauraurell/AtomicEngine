#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "Mesh.h"
#include "Texture.h"
#include <vector>
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

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

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);	

	return true;
}

bool ModuleImporter::Start() {

	LoadCheckerTexture();

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

void ModuleImporter::LoadMesh(char* file_path, string name)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh* myMesh = new Mesh();
			myMesh->filename = file_path;

			aiMesh* ourMesh = scene->mMeshes[i];

			myMesh->num_vertex = ourMesh->mNumVertices;
			myMesh->vertex = new float[myMesh->num_vertex * 3];
			memcpy(myMesh->vertex, ourMesh->mVertices, sizeof(float) * myMesh->num_vertex * 3);
			atLOG("New mesh with %d vertices", &myMesh->num_vertex);

			// copy faces
			if (ourMesh->HasFaces())
			{
				myMesh->num_index = ourMesh->mNumFaces * 3;
				myMesh->index = new uint[myMesh->num_index]; // assume each face is a triangle
				for (uint i = 0; i < ourMesh->mNumFaces; ++i)
				{
					if (ourMesh->mFaces[i].mNumIndices != 3)
					{
						atLOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&myMesh->index[i * 3], ourMesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

			if (ourMesh->HasNormals())
			{
				myMesh->num_normals = ourMesh->mNumVertices;
				myMesh->normals = new float[myMesh->num_normals * 3];

				for (unsigned int i = 0, v = 0; i < myMesh->num_normals; i++, v += 3)
				{
					myMesh->normals[v] = ourMesh->mNormals[i].x;
					myMesh->normals[v + 1] = ourMesh->mNormals[i].y;
					myMesh->normals[v + 2] = ourMesh->mNormals[i].z;
				}
			}

			if (ourMesh->HasVertexColors(0))
			{
				myMesh->num_colors = ourMesh->mNumVertices;
				myMesh->colors = new float[myMesh->num_colors * 4];

				for (unsigned int i = 0, v = 0; i < myMesh->num_colors; i++, v += 4)
				{
					myMesh->colors[v] = ourMesh->mColors[0][i].r;
					myMesh->colors[v + 1] = ourMesh->mColors[0][i].g;
					myMesh->colors[v + 2] = ourMesh->mColors[0][i].b;
					myMesh->colors[v + 3] = ourMesh->mColors[0][i].a;
				}
			}

			if (ourMesh->HasTextureCoords(0))
			{
				myMesh->num_texcoords = ourMesh->mNumVertices;
				myMesh->texcoords = new float[myMesh->num_texcoords * 2];

				for (unsigned int i = 0, v = 0; i < myMesh->num_texcoords; i++, v += 2)
				{
					myMesh->texcoords[v] = ourMesh->mTextureCoords[0][i].x;
					myMesh->texcoords[v + 1] = ourMesh->mTextureCoords[0][i].y;
				}
			}

			//aiVector3D position, scaling;
			//aiQuaternion rotation;

			//if(i == 2) scene->mRootNode->mChildren[i]->mTransformation.Decompose(scaling, rotation, position);
			//scaling.x = scaling.y = scaling.z = 1.0f;

			//float3 pos = float3(position.x, position.y, position.z);
			//Quat rot = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
			//float3 scale = float3(scaling.x, scaling.y, scaling.z);

			App->scene_intro->meshes.push_back(myMesh);

			for (int i = 0; i < App->scene_intro->meshes.size(); i++)
			{
				if (App->scene_intro->meshes[i] == myMesh) {
					GenerateBuffers(myMesh);
					if (ourMesh->mName != aiString("")) { App->scene_intro->CreateGameObject(App->scene_intro->meshes[i], ourMesh->mName.C_Str()); }
					else if (name != "none") { App->scene_intro->CreateGameObject(App->scene_intro->meshes[i], name); }
					else {
						App->scene_intro->CreateGameObject(App->scene_intro->meshes[i], "GameObject");
					}
				}
			}
		}

		aiReleaseImport(scene);
	}
	else
		atLOG("Error loading scene %s", file_path);
}

void ModuleImporter::LoadTexture(char* file_path)
{
	ILuint texture;
	ilGenImages(1, &texture);
	ilBindImage(texture);
	ilLoadImage(file_path);

	Gl_Tex = ilutGLBindTexImage();
	ilDeleteImages(1, &texture);

	if (texture != NULL)
	{
		if (App->gui->selectedObj != nullptr)
		{
			At_Tex* tex = new At_Tex(file_path, Gl_Tex);
			tex->w = ilGetInteger(IL_IMAGE_WIDTH) * 16;
			tex->h = ilGetInteger(IL_IMAGE_HEIGHT) * 16;
			App->scene_intro->texs.push_back(tex);
			for (int i = 0; i < App->scene_intro->texs.size(); i++)
			{
				if (App->scene_intro->texs[i] == tex) {
					if (App->gui->selectedObj->GetCMaterial() == nullptr)
					{
						App->gui->selectedObj->CreateComponent(ComponentType::Material);
					}
					if (App->gui->selectedObj->GetCMaterial()->hasTex == false)
					{
						App->gui->selectedObj->GetCMaterial()->tex = tex;
						App->gui->selectedObj->GetCMaterial()->tex->loaded = true;
						App->gui->selectedObj->GetCMaterial()->hasTex = true;
					}
				}
			}

			atLOG("Texture: %s loaded", file_path);
		}
		else
		{
			atLOG("Texture: %s loaded", file_path);
			atLOG("No GameObject was selected!")
		}
	}
	else {
		atLOG("Error loading the texture!");
	}
}

void ModuleImporter::LoadCheckerTexture()
{
	GLbyte checkerTex[64][64][4];
	GLuint GL_Tex_Checker;

	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerTex[i][j][0] = (GLubyte)c;
			checkerTex[i][j][1] = (GLubyte)c;
			checkerTex[i][j][2] = (GLubyte)c;
			checkerTex[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &GL_Tex_Checker);
	glBindTexture(GL_TEXTURE_2D, GL_Tex_Checker);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerTex);

	At_Tex* tex = new At_Tex(GL_Tex_Checker);
	App->scene_intro->texs.push_back(tex);
}

void ModuleImporter::GenerateBuffers(Mesh* m)
{
	m->id_vertex = 0;
	glGenBuffers(1, (GLuint*)&(m->id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, m->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_vertex * 3, m->vertex, GL_STATIC_DRAW);

	m->id_normals = 0;
	glGenBuffers(1, (GLuint*)&(m->id_normals));
	glBindBuffer(GL_ARRAY_BUFFER, m->id_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_normals * 3, &m->normals[0], GL_STATIC_DRAW);

	m->id_texcoords = 0;
	glGenBuffers(1, (GLuint*)&(m->id_texcoords));
	glBindBuffer(GL_ARRAY_BUFFER, m->id_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_vertex * 2, m->texcoords, GL_STATIC_DRAW);

	m->id_index = 0;
	glGenBuffers(1, ((GLuint*)&(m->id_index)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m->num_index, m->index, GL_STATIC_DRAW);
}

