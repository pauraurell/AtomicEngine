#pragma once
#include "Globals.h"
#include <string>

enum ResourceType
{
	MeshResource,
	ModelResource,
	MaterialResource,
	TextureResource,
	SceneResource,
};

class Resource
{
public:
	Resource(uint UID, ResourceType type);
	virtual ~Resource();

	uint uid = 0;
	ResourceType type;
};
