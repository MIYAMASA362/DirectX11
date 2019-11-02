#include"Common.h"
#include"Module\ECSEngine.h"

using namespace DirectX;

Object::Object()
:
	m_InstanceID(ObjectManager::AttachID())
{
	ObjectManager::AddIndex(std::shared_ptr<Object>(this));
}

Object::~Object()
{
	OnDestroy();
}

void Object::OnDestroy()
{

}

void Object::Destroy(Object* obj)
{
	obj->Destroy();
}

void DirectX::Object::Destroy()
{
	ObjectManager::AddDestroy(m_InstanceID);
}