#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "ModuleImporter.h"
#include "ModuleGUI.h"
#include <array>

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#pragma comment (lib, "Glew/libx86/glew32.lib") 

using namespace std;
#define MAX_LIGHTS 8

enum Primitives
{
	Cube,
	Sphere,
	pRectangle,
	Pyramid
};

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

	void RenderMesh(mesh* m);
	void LoadMeshBuffer();

	void CheckWireframeMode();

	void RenderPrimitive(Primitives type);

	//Primitives
	void DrawCube();
	void DrawRectangle();
	void DrawPyramid();

	void SetLight(bool enabled);
	void SetFaceCulling(bool enabled);
	void SetPolygonSmooth(bool enabled);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool wireframe_mode;

	bool cube_render;
	bool rectangle_render;
	bool pyramid_render;
};