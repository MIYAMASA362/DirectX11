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
//	�R���X�g���N�^
//
ComponentManager::ComponentManager()
{
	
}

//~ComponentManager
//	�f�X�g���N�^
//
ComponentManager::~ComponentManager()
{
	if(_EntityComponentIndex.size() != 0)_EntityComponentIndex.clear();
}




//Create
//	�C���X�^���X�̐���
//
void ComponentManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new ComponentManager();
	
}

//Release
//	�C���X�^���X�̔j��
//
void ComponentManager::Release()
{
	if (pInstance == nullptr) return;
	delete pInstance;
	pInstance = nullptr;
}





//SendComponentMessage
//	Component�ɑ΂���message���M
//
void ComponentManager::SendComponentMessage(std::string message)
{
	for (auto components : _EntityComponentIndex)
		for (auto component : components.second->_Components)
			component.second->SendComponentMessage(message);
}

//SendComponentMessage
//	�����Entity��Component�ɑ΂���message���M
//
void ComponentManager::SendComponentMessage(std::string message, EntityID entityID)
{
	for(auto component : _EntityComponentIndex.at(entityID)->_Components)
		component.second->SendComponentMessage(message);
}




//CreateComponents
//	Entity��Components���쐬
//
std::weak_ptr<ComponentList> ComponentManager::CreateComponents(IEntity* entity)
{
	return _EntityComponentIndex.emplace(entity->GetEntityID(), std::shared_ptr<ComponentList>(new ComponentList())).first->second;
}

//DestroyComponents
//	Entity�̎����Ă���Components�� Destroy�֐��𗘗p���č폜
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
//	ComponentList�̉��
//
void ComponentManager::ReleaseComponents(IEntity * entity)
{
	_EntityComponentIndex.erase(entity->GetEntityID());
}

//RegisterComponents
//	ComponentList��Entity�ɓo�^
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
//	Entity�̎����Ă���Components��Debug�\��
//
void ComponentManager::ImGui_ComponentView(EntityID id)
{
	for (auto component : _EntityComponentIndex.at(id)->_Components)
	{
		component.second->OnDebugImGui();
	}
}