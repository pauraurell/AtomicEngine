#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Globals.h"
#include "Module.h"
#include <vector>
#include <string>
#include "GameObject.h"
#include "ImGui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "MathGeoLib/MathGeoLib.h"

using namespace std;

class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);
	~ModuleGUI();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	update_status DockingSpace(bool* open);
	void AddGOtoHierarchy(GameObject* gameObject, int& id);

	void GameObjectPopUps();
	void ReparentDragDrop(GameObject* go, int& id);

	const char* GetCaps();
	void DrawUi();
	void SaveScene();
	void LoadScene();

	std::string caps;

	float brightness;
	int width;
	int height;
	bool wireframe_selected;
	std::vector<std::string> logs;

	bool lightChecker;
	bool smoothChecker;
	bool faceCullingChecker;

	bool gridChecker;
	bool bbChecker;
	bool rayChecker;
	int gridSize;

	float r;
	float g;
	float b;

	bool printInspector;
	GameObject* selectedObj;
	char* file_path;
	bool showImportWindow;
	float tempScale;
	float3 tempP;

	bool origin;
	bool custom_position;
	bool viewtransform;

	bool igLights;
	bool igCameras;

	char buff[256];

private:
	bool demowindow;
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fulldesktop;
	bool ConfigurationWindowActive;
	bool ConsoleWindowActive;
	bool AboutWindowActive;
	bool HierarchyWindowActive;
	bool AssetsWindowActive;
	bool InspectorWindowActive;
	bool* dockingWin;

	vector<float> fps_log;
	vector<float> ms_log;
	int fps;

	char folder_name[256];
	char file_name[128];
	char select_file[256];
};

#endif // __ModuleGUI_H__
