#include<map>
#include<memory>
#include"Module\Object\Object.h"
#include"IEntity.h"
#include"EntityManager.h"

using namespace DirectX;
EntityID EntityManager::m_EntityID;
std::map<EntityID, std::shared_ptr<IEntity>> EntityManager::EntityIndex;

EntityID EntityManager::AttachEntityID()
{
	EntityID id = m_EntityID;
	m_EntityID++;
	return id;
}
void EntityManager::Create()
{
	EntityIndex.clear();
	m_EntityID = 0;
}
void EntityManager::Release()
{
	EntityIndex.clear();
}
void EntityManager::RemoveEntity(EntityID id)
{
	EntityIndex.erase(id);
}
std::weak_ptr<IEntity> EntityManager::GetEntity(EntityID id)
{
	return EntityIndex.at(id);
}