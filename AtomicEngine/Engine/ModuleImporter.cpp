#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "Mesh.h"
#include "Texture.h"
#include "ModuleGUI.h"
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
	GameObject* root = nullptr;

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* rootNode = scene->mRootNode;
		root = PreorderChildren(scene, rootNode, nullptr, nullptr, file_path);
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

Mesh* ModuleImporter::LoadMesh(const aiScene* scene, aiNode* node, const char* path) {
	Mesh* currentMesh = new Mesh();
	aiMesh* currentAiMesh = scene->mMeshes[*node->mMeshes];

	//vertex copying
	currentMesh->num_vertex = currentAiMesh->mNumVertices;
	currentMesh->vertex = new float[currentMesh->num_vertex * 3]();
	memcpy(currentMesh->vertex, currentAiMesh->mVertices, sizeof(float) * currentMesh->num_vertex * 3);

	//indices copying
	if (currentAiMesh->HasFaces())
	{
		currentMesh->num_index = currentAiMesh->mNumFaces * 3;
		currentMesh->index = new uint[currentMesh->num_index]();

		for (size_t f = 0; f < currentAiMesh->mNumFaces; f++)
		{
			if (currentAiMesh->mFaces[f].mNumIndices != 3)
			{}
			else
			{
				memcpy(&currentMesh->index[f * 3], currentAiMesh->mFaces[f].mIndices, 3 * sizeof(uint));
			}
		}
	}

	currentMesh->texcoords = new float[currentMesh->num_vertex * 2]();
	currentMesh->colors = new float[currentMesh->num_index * 4]();
	currentMesh->normals = new float[currentAiMesh->mNumVertices * 3]();

	int t = 0;

	//normals, color and texture coordinates
	for (size_t v = 0, n = 0, tx = 0, c = 0; v < currentAiMesh->mNumVertices; v++, n += 3, c += 4, tx += 2)
	{
		//normal copying
		if (currentAiMesh->HasNormals())
		{
			currentMesh->num_normals = currentAiMesh->mNumVertices;
			currentMesh->normals[n] = currentAiMesh->mNormals[v].x;
			currentMesh->normals[n + 1] = currentAiMesh->mNormals[v].y;
			currentMesh->normals[n + 2] = currentAiMesh->mNormals[v].z;
		}
		//color copying
		if (currentAiMesh->HasVertexColors(0))
		{
			currentMesh->num_colors = currentAiMesh->mNumVertices;
			currentMesh->colors = new float[currentMesh->num_colors * 4];

			for (unsigned int i = 0, v = 0; i < currentMesh->num_colors; i++, v += 4)
			{
				currentMesh->colors[v] = currentAiMesh->mColors[0][i].r;
				currentMesh->colors[v + 1] = currentAiMesh->mColors[0][i].g;
				currentMesh->colors[v + 2] = currentAiMesh->mColors[0][i].b;
				currentMesh->colors[v + 3] = currentAiMesh->mColors[0][i].a;
			}
		}

		if (currentAiMesh->HasTextureCoords(0))
		{
			currentMesh->num_texcoords = currentAiMesh->mNumVertices;
			currentMesh->texcoords[tx] = currentAiMesh->mTextureCoords[0][v].x;
			currentMesh->texcoords[tx + 1] = currentAiMesh->mTextureCoords[0][v].y;
		}
		t = tx;
	}
	//LOG("Texcoords loaded: %d", t);
	GenerateBuffers(currentMesh);

	return currentMesh;
}

GameObject* ModuleImporter::PreorderChildren(const aiScene* scene, aiNode* node, aiNode* parentNode, GameObject* parentGameObject, char* path)
{
	
	GameObject* go = nullptr;

	if (node->mMeshes != nullptr)
	{
		Mesh* mesh = LoadMesh(scene, node, path);
		mesh->filename = path;
		go = App->scene_intro->CreateGameObject(mesh, path);

		LoadTransform(node, go->GetCTransform());
	}
	if (parentGameObject != nullptr)
	{
		parentGameObject->CreateChild(go);
		go->parent = parentGameObject;
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		PreorderChildren(scene, node->mChildren[i], node, go, path);
	}

	return go;
}

void ModuleImporter::LoadTransform(aiNode* node, ComponentTransform* transform)
{
	aiVector3D position, scaling, eulerRotation;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, position);
	eulerRotation = rotation.GetEuler() * RADTODEG;

	transform->SetPosition(position.x, position.y, position.z);
	//transform->SetRotation(eulerRotation.x, eulerRotation.y, eulerRotation.z);

	transform->SetScale(1, 1, 1);
	transform->UpdateLocalMatrix();
}



