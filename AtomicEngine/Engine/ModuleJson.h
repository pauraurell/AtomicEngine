#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Globals.h"
#include "Math.h"
#include "parson/parson.h"

class ConfigFile
{
public:
	ConfigFile();
	ConfigFile(const char* jsonName);
	ConfigFile(JSON_Object* section);
	~ConfigFile();

	uint Save(char** buffer) const;

	double GetNumber(const char* field, double default = 0.0, int index = -1) const;
	float GetFloat(const char* field, float default = 0.0f, int index = -1) const;
	bool GetBool(const char* field, bool default, int index = -1) const;
	int GetInt(const char* field, int default = 0, int index = -1) const;
	uint GetUInt(const char* field, uint default = 0, int index = -1) const;
	const char* GetString(const char* field, const char* default = nullptr, int index = -1) const;

	uint GetArraySize(const char* field) const;
	ConfigFile GetArray(const char* field, int index) const;

	bool AddFloat(const char* field, float value);
	bool AddBool(const char* field, bool value);
	bool AddInt(const char* field, int value);
	bool AddUInt(const char* field, uint value);
	bool AddString(const char* field, const char* string);
	bool AddArray(const char* array_name);
	bool AddArrayChild(const ConfigFile& config);
private:
	JSON_Value* FindValue(const char* field, int index) const;

private:

	JSON_Value* valueRoot = nullptr;
	JSON_Object* root = nullptr;
	JSON_Array* array = nullptr;

	bool to_delete = false;
};
#endif //__CONFIG_H__