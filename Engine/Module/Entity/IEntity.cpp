#include"Common.h"

#include"Module\ECSEngine.h"

using namespace DirectX;

IEntity::IEntity()
{
	EntityManager::CreateEntity(this);
}

DirectX::IEntity::~IEntity()
{
	EntityManager::ReleaseEntity(GetEntityID());
}

EntityID IEntity::GetEntityID()
{
	return this->GetInstanceID();
}

void DirectX::IEntity::DestroyComponents()
{
	ComponentManager::ReleaseComponents(GetEntityID());
}
