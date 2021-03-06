#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Globals.h"
#include "Module.h"
#include <vector>
#include <string>
#include "GameObject.h"
#include "ImGui/imgui.h"

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

	const char* GetCaps();
	void DrawUi();

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
	int gridSize;

	float r;
	float g;
	float b;

	bool printInspector;
	GameObject* selectedObj;

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
	bool InspectorWindowActive;
	bool* dockingWin;

	
	vector<float> fps_log;
	vector<float> ms_log;
	int fps;
};

#endif // __ModuleGUI_H__
