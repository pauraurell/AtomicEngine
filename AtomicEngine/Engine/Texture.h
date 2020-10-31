#include "Globals.h"
#include <string>
#include "Glew\include\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

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
	~At_Tex() {};

	char* texName;
	GLuint Gl_Tex;
	bool loaded = false;
};
