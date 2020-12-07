#include "ModuleJson.h"

//#include "mmgr/mmgr.h"

ConfigFile::ConfigFile()
{
	valueRoot = json_value_init_object();
	root = json_value_get_object(valueRoot);
	to_delete = true;
}


ConfigFile::ConfigFile(const char* jsonName)
{
	valueRoot = json_parse_string(jsonName);
	if (valueRoot != nullptr) {
		root = json_value_get_object(valueRoot);
		to_delete = true;
	}
}

ConfigFile::ConfigFile(JSON_Object* section) : root(section)
{}


ConfigFile::~ConfigFile()
{
	if (to_delete == true) {
		json_value_free(valueRoot);
	}
}

uint ConfigFile::Save(char** buffer) const
{
	uint written = json_serialization_size(valueRoot);
	*buffer = new char[written];
	json_serialize_to_buffer(valueRoot, *buffer, written);
	return written;
}

JSON_Value* ConfigFile::FindValue(const char* field, int index) const
{
	if (index < 0) {
		return json_object_get_value(root, field);
	}

	JSON_Array* array = json_object_get_array(root, field);
	if (array != nullptr) {
		return json_array_get_value(array, index);
	}
	return nullptr;
}

double ConfigFile::GetNumber(const char* field, double default, int index) const
{
	JSON_Value* value = FindValue(field, index);

	if (value && json_value_get_type(value) == JSONNumber) {
		return json_value_get_number(value);
	}

	return default;
}

float ConfigFile::GetFloat(const char* field, float default, int index) const
{
	JSON_Value* value = FindValue(field, index);

	if (value && json_value_get_type(value) == JSONNumber) {
		return (float)json_value_get_number(value);
	}

	return default;
}

bool ConfigFile::GetBool(const char* field, bool default, int index) const
{
	JSON_Value* value = FindValue(field, index);

	if (value && json_value_get_type(value) == JSONBoolean) {
		return json_value_get_boolean(value) != 0;
	}

	return default;
}

int ConfigFile::GetInt(const char* field, int default, int index) const
{
	JSON_Value* value = FindValue(field, index);

	if (value && json_value_get_type(value) == JSONNumber) {
		return (int)json_value_get_number(value);
	}

	return default;
}

uint ConfigFile::GetUInt(const char* field, uint default, int index) const
{
	JSON_Value* value = FindValue(field, index);

	if (value && json_value_get_type(value) == JSONNumber) {
		return (uint)json_value_get_number(value);
	}

	return default;
}

const char* ConfigFile::GetString(const char* field, const char* default, int index) const
{
	JSON_Value* value = FindValue(field, index);

	if (value && json_value_get_type(value) == JSONString) {
		return json_value_get_string(value);
	}

	return default;
}

uint ConfigFile::GetArraySize(const char* field) const
{
	JSON_Array* array = json_object_get_array(root, field);
	if (array == nullptr) {
		return -1;
	}
	int num = json_array_get_count(array);
	return num;
}

ConfigFile ConfigFile::GetArray(const char* field, int index) const
{
	JSON_Array* array = json_object_get_array(root, field);
	if (array != nullptr) {
		return ConfigFile(json_array_get_object(array, index));
	}
	return ConfigFile((JSON_Object*) nullptr);
}

bool ConfigFile::AddFloat(const char* field, float value)
{
	return json_object_set_number(root, field, (float)value) == JSONSuccess;
}

bool ConfigFile::AddBool(const char* field, bool value)
{
	return json_object_set_boolean(root, field, (value) ? 1 : 0) == JSONSuccess;
}

bool ConfigFile::AddInt(const char* field, int value)
{
	return json_object_set_number(root, field, (double)value) == JSONSuccess;
}

bool ConfigFile::AddUInt(const char* field, uint value)
{
	return json_object_set_number(root, field, (double)value) == JSONSuccess;
}

bool ConfigFile::AddString(const char* field, const char* string)
{
	return json_object_set_string(root, field, string) == JSONSuccess;
}

bool ConfigFile::AddArray(const char* array_name)
{
	JSON_Value* va = json_value_init_array();
	array = json_value_get_array(va);

	return json_object_set_value(root, array_name, va) == JSONSuccess;
}

bool ConfigFile::AddArrayChild(const ConfigFile& config)
{
	if (array != nullptr) {
		return json_array_append_value(array, json_value_deep_copy(config.valueRoot)) == JSONSuccess;
	}
	return false;
}
