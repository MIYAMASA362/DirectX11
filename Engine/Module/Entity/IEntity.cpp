#include<string>
#include<map>
#include<memory>
#include"Module\Object\Object.h"
#include"Module\Entity\EntityManager.h"
#include"IEntity.h"
#include"Entity.h"

using namespace DirectX;

IEntity::IEntity()
	:
	m_EntityID(EntityManager::AttachEntityID())
{

}

const EntityID IEntity::GetEntityID()
{
	return this->m_EntityID;
}

bool DirectX::IEntity::GetIsEnable()
{
	return this->m_IsEnable;
}
