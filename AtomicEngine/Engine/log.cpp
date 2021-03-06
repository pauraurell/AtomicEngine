#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);

	if (App != nullptr)
	{
		std::string text = std::string(tmp_string);
		if (&App->gui->logs != NULL) { App->gui->logs.push_back(text); }
		if (App->gui->logs.size() > 999) { App->gui->logs.front().erase(); }
	}
}