#include"Common.h"
#include<random>

#include"Module\ECSEngine.h"

using namespace DirectX;

EntityIndex EntityManager::m_EntityIndex;

EntityID EntityManager::AttachEntityID()
{
	std::random_device rand;
	EntityID id;
	while(true){
		id = rand();
		if (m_EntityIndex.find(id) == m_EntityIndex.end())
			break;
	}

	return id;
}
void EntityManager::Create()
{
	m_EntityIndex.clear();
}
void EntityManager::Release()
{
	m_EntityIndex.clear();
}
void EntityManager::RemoveEntity(EntityID id)
{
	m_EntityIndex.erase(id);
}
std::weak_ptr<IEntity> EntityManager::GetEntity(EntityID id)
{
	return m_EntityIndex.at(id);
}