#include"Common.h"
#include<random>

#include"Module\ECSEngine.h"

using namespace DirectX;

//EntityにアタッチされたComponentのリスト
DirectX::EntityComponents DirectX::ComponentManager::EntityComponentIndex;

void ComponentManager::Create(){
	EntityComponentIndex.clear();
}

void ComponentManager::Release(){
	EntityComponentIndex.clear();
}

ComponentTypeID ComponentManager::AttachComponentTypeID(){
	std::random_device rand;
	return rand();
}

void DirectX::ComponentManager::SendComponentMessage(std::string message)
{
	auto itr = EntityComponentIndex.begin();
	auto end = EntityComponentIndex.end();

	while (itr != end) {
		auto citr = itr->second.get()->begin();
		auto cend = itr->second.get()->end();
		while (citr != cend) {
			citr->second.lock()->SendComponentMessage(message);
			citr++;
		}
		itr++;
	}
}

std::weak_ptr<Components> ComponentManager::GetComponents(EntityID id)
{
	if (EntityComponentIndex.size() == 0)
		return std::weak_ptr<Components>();
	return EntityComponentIndex.at(id);
}

void DirectX::ComponentManager::DestroyComponents(EntityID id)
{
	auto index = EntityComponentIndex.at(id);
	auto end = index->end();
	auto itr = index->begin();
	while (itr != end) {
		itr->second.lock()->Destroy();
		itr++;
	}
}

void DirectX::ComponentManager::ReleaseComponents(EntityID id)
{
	EntityComponentIndex.at(id)->clear();
}
