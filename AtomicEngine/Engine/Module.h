#pragma once

class Application;

#include <string>
class ConfigFile;

class Module
{
private :
	bool enabled;

public:
	Application* App;
	std::string name;

	Module(Application* parent, bool start_enabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual bool LoadConfig(ConfigFile* data)
	{
		return true;
	}

	virtual bool SaveConfig(ConfigFile* data) const
	{
		return true;
	}
};