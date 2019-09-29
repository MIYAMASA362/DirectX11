#include<string>
#include<map>
#include<memory>
#include<list>
#include<vector>

#include"Module\ECSEngine.h"

using namespace DirectX;

IEntity::IEntity()
	:
	m_EntityID(EntityManager::AttachEntityID())
{

}

DirectX::IEntity::~IEntity()
{
	this->DestroyComponents();
}

const EntityID IEntity::GetEntityID()
{
	return this->m_EntityID;
}

bool DirectX::IEntity::GetIsEnable()
{
	return this->m_IsEnable;
}

void DirectX::IEntity::DestroyComponents()
{
	ComponentManager::DestroyComponents(this->m_EntityID);
}