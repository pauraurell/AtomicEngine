#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Globals.h"
#include "Module.h"
#include "ImGui/imgui.h"
#include <vector>
#include <string>

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

private:
	bool demowindow;
	bool aboutwindow;
	bool consolewindow;
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fulldesktop;

};

#endif // __ModuleGUI_H__
