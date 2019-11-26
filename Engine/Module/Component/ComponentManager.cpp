#include"Common.h"
#include<random>

#include"Module\ECSEngine.h"

using namespace DirectX;

EntityComponents	ComponentManager::_EntityComponentIndex;
ComponentTypeIndex	ComponentManager::_ComponentTypeIndex;

void ComponentManager::Create(){
	_EntityComponentIndex.clear();
	_ComponentTypeIndex.clear();
}

void ComponentManager::Release(){
	_EntityComponentIndex.clear();
	_ComponentTypeIndex.clear();
}

//Component毎のIDを設定
ComponentTypeID ComponentManager::AttachComponentTypeID(){
	std::random_device rand;
	return rand();
}

//Componentsの生成・取得
std::weak_ptr<Components> ComponentManager::GetOrCreateComponentIndex(ComponentTypeID componentTypeID)
{
	auto find = _ComponentTypeIndex.find(componentTypeID);
	if (find != _ComponentTypeIndex.end())
		return find->second;

	auto instance = _ComponentTypeIndex.emplace(componentTypeID,std::shared_ptr<Components>(new Components()));
	return instance.first->second;
}

//Componentsの取得
std::weak_ptr<Components> DirectX::ComponentManager::GetComponentIndex(ComponentTypeID componentTypeID)
{
	auto find = _ComponentTypeIndex.find(componentTypeID);
	if (find == _ComponentTypeIndex.end())
		return std::weak_ptr<Components>();
	return find->second;
}

//ReleaseComponents
void DirectX::ComponentManager::ReleaseComponentIndex(ComponentTypeID componentTypeID)
{
	_ComponentTypeIndex.erase(componentTypeID);
}

//Componentに対してmessage送信
void DirectX::ComponentManager::SendComponentMessage(std::string message)
{
	for (auto components : _EntityComponentIndex)
		for (auto component : *components.second)
			component.lock()->SendComponentMessage(message);
}

//特定のEntityのComponentに対してmessage送信
void DirectX::ComponentManager::SendComponentMessage(std::string message, EntityID entityID)
{
	for(auto component : *_EntityComponentIndex.at(entityID))
		component.lock()->SendComponentMessage(message);
}

//
std::weak_ptr<IComponent> DirectX::ComponentManager::AddComponentIndex(std::weak_ptr<Object> object)
{
	//IComponentの設定
	auto sptr = std::dynamic_pointer_cast<IComponent>(object.lock());
	sptr->_self = sptr;

	//EntityComponentIndexに追加
	auto find = _EntityComponentIndex.find(sptr->GetOwnerID());
	if (find == _EntityComponentIndex.end())
		find = _EntityComponentIndex.emplace(sptr->GetOwnerID(),std::shared_ptr<Components>(new Components())).first;
	find->second->push_back(sptr);

	//ComponentTypeIndexに追加
	_ComponentTypeIndex.at(sptr->GetComponentTypeID())->push_back(sptr);

	return sptr;
}

//Entityの持っているComponentsを削除
void DirectX::ComponentManager::DestroyComponents(EntityID id)
{
	for(auto component : *_EntityComponentIndex.at(id))
		component.lock()->Destroy();
}

//Entityの持っているComponentsを開放
void DirectX::ComponentManager::ReleaseComponents(EntityID id)
{
	_EntityComponentIndex.erase(id);
}

//Entityの持っているComponentsのDebug表示
void DirectX::ComponentManager::ImGui_ComponentView(EntityID id)
{
	for (auto component : *_EntityComponentIndex.at(id))
		component.lock()->OnDebugImGui();
}
