#include "Globals.h"
#include <string>
using namespace std;

class Texture {
public:
	Texture(uint id, const char* path);

	uint id;
	string path;
};
