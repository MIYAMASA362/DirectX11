#include"Common.h"
#include<random>

#include"Module\ECSEngine.h"

using namespace DirectX;

ObjectIndex ObjectManager::m_ObjectIndex;
DestroyIndex ObjectManager::m_DestroyIndex;

InstanceID DirectX::ObjectManager::AttachID()
{
	std::random_device rand;
	return rand();
}

void DirectX::ObjectManager::AddIndex(std::shared_ptr<Object> instance)
{
	m_ObjectIndex.emplace(instance->GetInstanceID(),instance);
}

void DirectX::ObjectManager::AddDestroy(InstanceID instanceID)
{
	m_DestroyIndex.push_back(instanceID);
}

void DirectX::ObjectManager::ClearnUp()
{
	if (m_DestroyIndex.size() == 0) return;
	for (auto id : m_DestroyIndex) 
	{
		auto sptr = m_ObjectIndex.at(id);
		m_ObjectIndex.erase(id);
	}
	m_DestroyIndex.clear();
}

std::weak_ptr<Object> DirectX::ObjectManager::GetInstance(InstanceID instanceID)
{
	return m_ObjectIndex.at(instanceID);
}

void DirectX::ObjectManager::Release()
{
	m_ObjectIndex.clear();
}

void DirectX::ObjectManager::RemoveIndex(InstanceID instanceID)
{
	m_ObjectIndex.erase(instanceID);
}
