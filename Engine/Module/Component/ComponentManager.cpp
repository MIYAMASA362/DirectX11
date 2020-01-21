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
	_EntityComponentIndex.clear();
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
		for (auto component : components.second->_components)
			component.lock()->SendComponentMessage(message);
}

//SendComponentMessage
//	特定のEntityのComponentに対してmessage送信
//
void ComponentManager::SendComponentMessage(std::string message, EntityID entityID)
{
	for(auto component : _EntityComponentIndex.at(entityID)->_components)
		component.lock()->SendComponentMessage(message);
}




//CreateComponents
//	EntityのComponentsを作成
//
std::weak_ptr<ComponentList> ComponentManager::CreateComponents(IEntity* entity)
{
	return _EntityComponentIndex.emplace(entity->GetEntityID(), std::shared_ptr<ComponentList>(new ComponentList())).first->second;
}

//GetComponents
//	Entityの持っているComponentsを取得
//
std::weak_ptr<ComponentList> ComponentManager::GetComponents(IEntity* entity)
{
	return _EntityComponentIndex.at(entity->GetEntityID());
}

//DestroyComponents
//	Entityの持っているComponentsを Destroy関数を利用して削除
//
void ComponentManager::DestroyComponents(IEntity* entity)
{
	for(auto component : _EntityComponentIndex.at(entity->GetEntityID())->_components)
		component.lock()->Destroy();
}

//ReleaseComponents
//	Entityの持っているComponentsを開放
//
void ComponentManager::ReleaseComponents(IEntity* entity)
{
	_EntityComponentIndex.erase(entity->GetEntityID());
}





//ImGui_ComponentView
//	Entityの持っているComponentsのDebug表示
//
void ComponentManager::ImGui_ComponentView(EntityID id)
{
	for (auto component : _EntityComponentIndex.at(id)->_components)
	{
		component.lock()->OnDebugImGui();
	}
}