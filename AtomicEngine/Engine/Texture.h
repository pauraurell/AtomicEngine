#pragma once

#include "Globals.h"
#include "Glew\include\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Devil\Include\il.h"
#include "Devil\Include\ilu.h"
#include "Devil\Include\ilut.h"

#pragma comment (lib, "Devil/lib/ILU.lib") 
#pragma comment (lib, "Devil/lib/ILUT.lib") 
#pragma comment (lib, "Devil/lib/DevIL.lib") 

using namespace std;

class At_Tex
{
public:
	At_Tex() {};
	At_Tex(char* filename, GLuint Gl_Text)
	{
		texName = filename;
		Gl_Tex = Gl_Text;
	}

	At_Tex(GLuint Gl_Text)
	{
		Gl_Tex = Gl_Text;
	}
	~At_Tex() {};

	char* texName = nullptr;
	GLuint Gl_Tex = 0;
	bool loaded = false;
	bool checkers = false;
	ILuint w;
	ILuint h;
};
