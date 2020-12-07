#include "Resource.h"

Resource::Resource(uint UID, ResourceType type) :  uid(UID), type(type)
{
	uid = UID;
	type = type;
}

Resource::~Resource()
{

}