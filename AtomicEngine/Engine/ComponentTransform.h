#pragma once
#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	void Enable();
	void Update();
	void Disable();
};

#endif //__COMPONENTTRANSFORM_H__
