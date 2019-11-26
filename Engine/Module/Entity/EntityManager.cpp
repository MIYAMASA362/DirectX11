#include"Common.h"
#include"Module\ECSEngine.h"

using namespace DirectX;

EntityIndex EntityManager::m_EntityIndex;

void EntityManager::Create()
{
	m_EntityIndex.clear();
}

void EntityManager::Release()
{
	m_EntityIndex.clear();
}

std::weak_ptr<IEntity> EntityManager::CreateEntity(IEntity* instance)
{
	InstanceID id = instance->GetInstanceID();
	auto object = ObjectManager::GetInstance(id).lock();
	auto wptr = std::weak_ptr<IEntity>(std::dynamic_pointer_cast<IEntity>(object));
	m_EntityIndex.emplace(id,wptr);
	return wptr;
}

std::weak_ptr<IEntity> EntityManager::GetEntity(EntityID id)
{
	auto find = m_EntityIndex.find(id);
	if (find == m_EntityIndex.end()) 
		return std::weak_ptr<IEntity>();
	return find->second;
}

void EntityManager::ReleaseEntity(EntityID id)
{
	m_EntityIndex.erase(id);
	ComponentManager::ReleaseComponents(id);
}