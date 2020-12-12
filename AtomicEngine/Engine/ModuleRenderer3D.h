#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "ModuleImporter.h"
#include "ModuleGUI.h"
#include <array>
#include "ModuleGui.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#pragma comment (lib, "Glew/libx86/glew32.lib") 

using namespace std;
#define MAX_LIGHTS 8

class Mesh;
class At_Tex;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);

	void RenderGameObject(GameObject* game_object);

	void CheckWireframeMode();

	//Primitives
	void DrawCube();
	void DrawCubeDirectMode();
	void DrawCubeVerticeArray();

	void SetLight(bool enabled);
	void SetFaceCulling(bool enabled);
	void SetPolygonSmooth(bool enabled);

	void RenderVertexNormals(Mesh* m);
	void RenderFaceNormals(Mesh* m);

	void UpdateProjMat(float* projMat);
	void DrawAABB(float3* aabb);
	bool FrustumCulling(GameObject* go);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool wireframe_mode;
};