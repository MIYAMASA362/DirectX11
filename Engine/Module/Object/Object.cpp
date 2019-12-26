#include"Common.h"

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Object.h"
#include"ObjectManager.h"

Object::Object()
:
	_InstanceID(ObjectManager::AttachID())
{
	_self = ObjectManager::AddIndex(this);
}

Object::~Object()
{
	
}

void Object::OnDestroy()
{

}

void Object::Destroy(Object* obj)
{
	obj->Destroy();
}

void Object::Destroy()
{
	ObjectManager::AddDestroy(this);
}