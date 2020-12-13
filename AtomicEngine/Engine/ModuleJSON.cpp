#include "ModuleJSON.h"
#include "parson/parson.h"

//GnJSONObject ====================================================

GnJSONObj::GnJSONObj()
{
	root = json_value_init_object();
	objct = json_value_get_object(root);
}

GnJSONObj::GnJSONObj(const char* buffer) : objct(nullptr)
{
	root = json_parse_string(buffer);

	if (root != NULL)
	{
		objct = json_value_get_object(root);
	}
	else
	{
		//LOG("Error trying to load json file");
	}
}

GnJSONObj::GnJSONObj(JSON_Object* object) : root(nullptr)
{
	objct = object;
}

GnJSONObj::~GnJSONObj()
{
	objct = nullptr;

	if (root != nullptr)
	{
		//json_value_free(_root);
		root = nullptr;
	}
}

void GnJSONObj::Release()
{
	json_value_free(root);
}

JSON_Object* GnJSONObj::GetJSONObject()
{
	return objct;
}

JSON_Value* GnJSONObj::GetValue()
{
	return root;
}

uint GnJSONObj::Save(char** buffer)
{
	uint size = json_serialization_size_pretty(root);
	*buffer = new char[size];
	json_serialize_to_buffer_pretty(root, *buffer, size);
	return size;
}

JSON_Array* GnJSONObj::GetArray(const char* name)
{
	if (json_object_has_value(objct, name) == 1)
		return json_object_get_array(objct, name);
	else
		return nullptr;
}

int GnJSONObj::GetInt(const char* name, int default)
{
	if (json_object_has_value(objct, name) == 1)
		return json_object_get_number(objct, name);
	else
		return default;
}

float GnJSONObj::GetFloat(const char* name, float default)
{
	if (json_object_has_value(objct, name) == 1)
		return json_object_get_number(objct, name);
	else
		return default;
}

bool GnJSONObj::GetBool(const char* name, bool default)
{
	if (json_object_has_value(objct, name) == 1)
		return json_object_get_boolean(objct, name);
	else
		return default;
}

const char* GnJSONObj::GetString(const char* name, const char* default)
{
	if (json_object_has_value(objct, name) == 1)
		return json_object_get_string(objct, name);
	else
		return default;
}

math::float3 GnJSONObj::GetFloat3(const char* name, float3 default)
{
	if (json_object_has_value(objct, name) == 1)
	{
		math::float3 number;

		JSON_Array* array;
		array = json_object_get_array(objct, name);

		number.x = json_array_get_number(array, 0);
		number.y = json_array_get_number(array, 1);
		number.z = json_array_get_number(array, 2);

		return number;
	}
	else
		return default;
}

Quat GnJSONObj::GetQuaternion(const char* name, Quat default)
{
	if (json_object_has_value(objct, name) == 1)
	{
		math::Quat quaternion;

		JSON_Array* array;
		array = json_object_get_array(objct, name);

		quaternion.x = json_array_get_number(array, 0);
		quaternion.y = json_array_get_number(array, 1);
		quaternion.z = json_array_get_number(array, 2);
		quaternion.w = json_array_get_number(array, 3);

		return quaternion;
	}
	else
		return default;
}

Color GnJSONObj::GetColor(const char* name, Color default)
{
	if (json_object_has_value(objct, name) == 1)
	{
		Color color;

		JSON_Array* array;
		array = json_object_get_array(objct, name);

		color.r = json_array_get_number(array, 0);
		color.g = json_array_get_number(array, 1);
		color.b = json_array_get_number(array, 2);

		return color;
	}
	else
		return default;
}

void GnJSONObj::AddInt(const char* name, int number)
{
	json_object_set_number(objct, name, number);
}

void GnJSONObj::AddFloat(const char* name, float number)
{
	json_object_set_number(objct, name, number);
}

void GnJSONObj::AddFloat3(const char* name, math::float3 number)
{
	JSON_Value* value = json_value_init_array();
	JSON_Status status = json_object_set_value(objct, name, value);
	JSON_Array* _array = json_object_get_array(objct, name);

	json_array_append_number(_array, number.x);
	json_array_append_number(_array, number.y);
	json_array_append_number(_array, number.z);
}

void GnJSONObj::AddQuaternion(const char* name, math::Quat number)
{
	JSON_Value* value = json_value_init_array();
	JSON_Status status = json_object_set_value(objct, name, value);
	JSON_Array* _array = json_object_get_array(objct, name);

	json_array_append_number(_array, number.x);
	json_array_append_number(_array, number.y);
	json_array_append_number(_array, number.z);
	json_array_append_number(_array, number.w);
}

void GnJSONObj::AddBool(const char* name, bool boolean)
{
	json_object_set_boolean(objct, name, boolean);
}

void GnJSONObj::AddString(const char* name, const char* string)
{
	json_object_set_string(objct, name, string);
}

void GnJSONObj::AddColor(const char* name, Color color)
{
	JSON_Value* value = json_value_init_array();
	JSON_Status status = json_object_set_value(objct, name, value);
	JSON_Array* _array = json_object_get_array(objct, name);

	json_array_append_number(_array, color.r);
	json_array_append_number(_array, color.g);
	json_array_append_number(_array, color.b);
}

GnJSONArray GnJSONObj::AddArray(const char* name)
{
	JSON_Value* value = json_value_init_array();
	JSON_Status status = json_object_set_value(objct, name, value);

	if (status == JSONFailure)
	{
		json_value_free(value);
	}
	else
	{
		return GnJSONArray(json_object_get_array(objct, name));
	}
}

GnJSONArray GnJSONObj::AddArray(GnJSONArray array)
{
	return GnJSONArray();
}

//GnJSONArray =====================================================

GnJSONArray::GnJSONArray() : Array(nullptr), Nested(false)
{
	Value = json_value_init_array();
	json_value_get_array(Value);
}

GnJSONArray::GnJSONArray(JSON_Array* array, JSON_Object* root_object) : Array(array) {}

GnJSONArray::GnJSONArray(JSON_Array* array) : Array(array), Value(nullptr) {}

GnJSONArray::GnJSONArray(const char* name) : Nested(false)
{
	JSON_Value* value = json_value_init_array();
	json_value_get_array(value);
}

GnJSONArray::~GnJSONArray()
{
	Array = nullptr;
}

int GnJSONArray::Size()
{
	return json_array_get_count(Array);
}

GnJSONObj GnJSONArray::GetObjectInArray(const char* name)
{
	for (size_t i = 0; i < json_array_get_count(Array); i++)
	{
		JSON_Object* object = json_array_get_object(Array, i);
		const char* object_name = json_object_get_string(object, "name");

		if (strcmp(name, object_name) == 0)
			return GnJSONObj(object);
	}

	//LOG("JSON object %s could not be found", name);
}

GnJSONObj GnJSONArray::GetObjectAt(int index)
{
	JSON_Object* object = json_array_get_object(Array, index);
	return GnJSONObj(object);
}

void GnJSONArray::AddObject(GnJSONObj& object)
{
	json_array_append_value(Array, object.GetValue());
}

void GnJSONArray::AddInt(int number)
{
	json_array_append_number(Array, number);
}

void GnJSONArray::AddFloat(float number)
{
	json_array_append_number(Array, number);
}

void GnJSONArray::AddString(const char* string)
{
	json_array_append_string(Array, string);
}