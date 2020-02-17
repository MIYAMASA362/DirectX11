#include"Common.h"
#include<algorithm>
#include<random>
#include<functional>

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Object\Object.h"
#include"Module\Object\ObjectManager.h"

#include"IComponent.h"
#include"IComponentSystem.h"

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
//	
//
void ComponentManager::SendComponentMessage(std::string message)
{
	for(auto component : _ComponentIndex)
	{
		component.lock()->SendComponentMessage(message);
	}
}

void ComponentManager::RegisterComponent(std::weak_ptr<IComponent> component)
{
	_ComponentIndex.push_back(component);
}

void ComponentManager::ReleaseComponent(IComponent * component)
{
	ComponentID id = component->GetComponentID();

	auto end = _ComponentIndex.end();
	auto find = std::remove_if(
		_ComponentIndex.begin(), end, [id](std::weak_ptr<IComponent> component) 
	{
		return component.lock()->GetComponentID() == id;
	});
	_ComponentIndex.erase(find,end);
}
