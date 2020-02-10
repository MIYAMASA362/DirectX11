#include"Common.h"
#include<random>
#include<functional>

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Object\Object.h"
#include"Module\Object\ObjectManager.h"

#include"IComponent.h"
#include"ComponentList.h"

#include"ComponentManager.h"
#include"Module\Entity\IEntity.h"


//*********************************************************************************************************************
//
//	ComponentManager
//
//*********************************************************************************************************************

std::unordered_map<EntityID, std::shared_ptr<ComponentList>> ComponentManager::_EntityComponentIndex;

//pInstance
//	Singleton
//
ComponentManager * ComponentManager::pInstance = nullptr;



//ComponentManager
//	コンストラクタ
//
ComponentManager::ComponentManager()
{
	
}

//~ComponentManager
//	デストラクタ
//
ComponentManager::~ComponentManager()
{
	if(_EntityComponentIndex.size() != 0)_EntityComponentIndex.clear();
}




//Create
//	インスタンスの生成
//
void ComponentManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new ComponentManager();
	
}

//Release
//	インスタンスの破棄
//
void ComponentManager::Release()
{
	if (pInstance == nullptr) return;
	delete pInstance;
	pInstance = nullptr;
}





//SendComponentMessage
//	Componentに対してmessage送信
//
void ComponentManager::SendComponentMessage(std::string message)
{
	for (auto components : _EntityComponentIndex)
		for (auto component : components.second->_Components)
			component.second->SendComponentMessage(message);
}

//SendComponentMessage
//	特定のEntityのComponentに対してmessage送信
//
void ComponentManager::SendComponentMessage(std::string message, EntityID entityID)
{
	for(auto component : _EntityComponentIndex.at(entityID)->_Components)
		component.second->SendComponentMessage(message);
}




//CreateComponents
//	EntityのComponentsを作成
//
std::weak_ptr<ComponentList> ComponentManager::CreateComponents(IEntity* entity)
{
	return _EntityComponentIndex.emplace(entity->GetEntityID(), std::shared_ptr<ComponentList>(new ComponentList())).first->second;
}

//DestroyComponents
//	Entityの持っているComponentsを Destroy関数を利用して削除
//
void ComponentManager::DestroyComponents(IEntity* entity)
{
	auto find = _EntityComponentIndex.at(entity->GetEntityID());

	for(auto component : find->_Components)
	{
		component.second->Destroy();
	}
}

//ReleaseComponents
//	ComponentListの解放
//
void ComponentManager::ReleaseComponents(IEntity * entity)
{
	_EntityComponentIndex.erase(entity->GetEntityID());
}

//RegisterComponents
//	ComponentListをEntityに登録
//
std::weak_ptr<ComponentList> ComponentManager::SwapComponents(IEntity* entity,ComponentList * Components)
{
	_EntityComponentIndex.erase(entity->GetEntityID());
	return _EntityComponentIndex.emplace(entity->GetEntityID(),std::shared_ptr<ComponentList>(Components)).first->second;
}

std::weak_ptr<ComponentList> ComponentManager::SwapComponents(IEntity * entity, std::shared_ptr<ComponentList> Components)
{
	_EntityComponentIndex.erase(entity->GetEntityID());
	return _EntityComponentIndex.emplace(entity->GetEntityID(), Components).first->second;
}

//ImGui_ComponentView
//	Entityの持っているComponentsのDebug表示
//
void ComponentManager::ImGui_ComponentView(EntityID id)
{
	for (auto component : _EntityComponentIndex.at(id)->_Components)
	{
		component.second->OnDebugImGui();
	}
}