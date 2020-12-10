#pragma once
#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Component.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	//ComponentCamera();
	~ComponentCamera();

	void Enable();
	void Update();
	void Disable();

};

#endif //__COMPONENTCAMERA_H__
