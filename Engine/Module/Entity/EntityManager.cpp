#include"Common.h"
#include<random>

#include"Module\ECSEngine.h"

using namespace DirectX;

EntityIndex EntityManager::m_EntityIndex;

EntityID EntityManager::AttachEntityID()
{
	std::random_device rand;
	EntityID id;
	while (true)
	{
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
	//IEntity->OnDestroy();
	m_EntityIndex.at(id)->OnDestroy();
	m_EntityIndex.erase(id);
}

void DirectX::EntityManager::IndexClearnUp()
{
	
	auto itr = m_EntityIndex.begin();
	auto end = m_EntityIndex.end();

	while (itr != end) {
		if (!itr->second->IsDestroy()) {
			itr++;
			continue;
		}
		itr->second->OnDestroy();
		itr = m_EntityIndex.erase(itr);
	}
}

std::weak_ptr<IEntity> EntityManager::GetEntity(EntityID id)
{
	return m_EntityIndex.at(id);
}