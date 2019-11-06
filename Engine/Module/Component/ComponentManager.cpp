#include"Common.h"
#include<random>

#include"Module\ECSEngine.h"

using namespace DirectX;

//EntityにアタッチされたComponentのリスト
DirectX::EntityComponents DirectX::ComponentManager::EntityComponentIndex;
std::unordered_map<ComponentTypeID, std::shared_ptr<ComponentIndex>> ComponentManager::_ComponentTypeIndex;

void ComponentManager::Create(){
	EntityComponentIndex.clear();
}

void ComponentManager::Release(){
	EntityComponentIndex.clear();
	_ComponentTypeIndex.clear();
}

ComponentTypeID ComponentManager::AttachComponentTypeID(){
	std::random_device rand;
	return rand();
}

std::weak_ptr<ComponentIndex> DirectX::ComponentManager::GetOrCreateComponentIndex(ComponentTypeID componentTypeID)
{
	auto find = _ComponentTypeIndex.find(componentTypeID);
	if (find != _ComponentTypeIndex.end())
		return find->second;

	auto instance = _ComponentTypeIndex.emplace(componentTypeID,std::shared_ptr<ComponentIndex>(new ComponentIndex()));
	return instance.first->second;
}

void DirectX::ComponentManager::ReleaseComponentIndex(ComponentTypeID componentTypeID)
{
	_ComponentTypeIndex.erase(componentTypeID);
}

void DirectX::ComponentManager::SendComponentMessage(std::string message)
{
	for(auto components : EntityComponentIndex)
		for(auto component : *components.second)
			component.second.lock()->SendComponentMessage(message);
}

std::weak_ptr<Components> ComponentManager::GetComponents(EntityID id)
{
	return EntityComponentIndex.at(id);
}

void DirectX::ComponentManager::DestroyComponents(EntityID id)
{
	for(auto component : *EntityComponentIndex.at(id))
		component.second.lock()->Destroy();
}

void DirectX::ComponentManager::ReleaseComponents(EntityID id)
{
	EntityComponentIndex.erase(id);
}

void DirectX::ComponentManager::ImGui_ComponentView(EntityID id)
{
	for (auto component : *EntityComponentIndex.at(id))
		component.second.lock()->OnDebugImGui();
}
