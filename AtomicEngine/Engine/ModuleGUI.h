#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Globals.h"
#include "Module.h"
#include <vector>
#include <string>
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

	float brightness;
	int width;
	int height;

private:
	bool demowindow;
	bool aboutwindow;
	bool consolewindow;
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fulldesktop;
	bool ConfigurationWindowActive;
	bool ConsoleWindowActive;

	vector<float> fps_log;
	vector<float> ms_log;
	int fps;
};

#endif // __ModuleGUI_H__