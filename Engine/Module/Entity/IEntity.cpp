#include"Common.h"

#include"Module\ECSEngine.h"

using namespace DirectX;

IEntity::IEntity()
	:
	m_EntityID(EntityManager::AttachEntityID())
{

}

DirectX::IEntity::~IEntity()
{
	ComponentManager::DestroyComponents(this->m_EntityID);
}

const EntityID IEntity::GetEntityID()
{
	return this->m_EntityID;
}

bool DirectX::IEntity::GetIsEnable()
{
	return this->m_IsEnable;
}