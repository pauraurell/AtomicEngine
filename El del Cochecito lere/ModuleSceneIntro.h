#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Timer.h"

#include "PugiXml\src\pugixml.hpp"


struct Timer;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Awake();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	
private:
	pugi::xml_document map_file;
	pugi::xml_node map_node;

	pugi::xml_node LoadMap(pugi::xml_document&) const;

};
