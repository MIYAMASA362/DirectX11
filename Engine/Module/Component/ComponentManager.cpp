#include"Common.h"
#include<random>

#include"Module\ECSEngine.h"

using namespace DirectX;

std::vector<ComponentID> ComponentManager::IdIndex;
//EntityにアタッチされたComponentのリスト
DirectX::EntityComponents DirectX::ComponentManager::EntityComponentIndex;

void ComponentManager::DestroyComponents(EntityID id)
{
	if (EntityComponentIndex.find(id) == EntityComponentIndex.end()) return;
	//iterator
	auto itr = EntityComponentIndex.at(id)->begin();
	auto end = EntityComponentIndex.at(id)->end();
	
	while (itr != end) {
		itr->second->OnDestroy();
		itr->second->Destroy();
		itr = EntityComponentIndex.at(id)->erase(itr);
	}
}

void DirectX::ComponentManager::SendComponentMessage(std::string message)
{
	auto itr = EntityComponentIndex.begin();
	auto end = EntityComponentIndex.end();

	while (itr != end) {
		auto citr = itr->second.get()->begin();
		auto cend = itr->second.get()->end();
		while (citr != cend) {
			citr->second.get()->SendComponentMessage(message);
			citr++;
		}
		itr++;
	}
}

ComponentID ComponentManager::CreateComponent()
{
	std::random_device rand;
	ComponentID id;
	while (true) {
		id = rand();
		auto itr = std::find(IdIndex.begin(), IdIndex.end(),id);
		if (itr == IdIndex.end())
			break;
	}
	IdIndex.push_back(id);
	return id;
}