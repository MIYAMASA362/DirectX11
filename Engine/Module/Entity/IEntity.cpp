#include"Common.h"

#include"Module\ECSEngine.h"
using namespace DirectX;

IEntity::IEntity()
{
	_self = EntityManager::CreateEntity(this);
}

DirectX::IEntity::~IEntity()
{
	EntityManager::ReleaseEntity(GetEntityID());
}

EntityID IEntity::GetEntityID()
{
	return this->GetInstanceID();
}

std::weak_ptr<IEntity> DirectX::IEntity::GetEntity()
{
	return this->_self;
}

void DirectX::IEntity::DestroyComponents()
{
	ComponentManager::DestroyComponents(GetEntityID());
}

void DirectX::IEntity::Destroy()
{
	ComponentManager::DestroyComponents(GetEntityID());
	Object::Destroy();
}
