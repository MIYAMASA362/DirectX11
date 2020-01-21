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
	_EntityComponentIndex.clear();
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
		for (auto component : components.second->_components)
			component.lock()->SendComponentMessage(message);
}

//SendComponentMessage
//	�����Entity��Component�ɑ΂���message���M
//
void ComponentManager::SendComponentMessage(std::string message, EntityID entityID)
{
	for(auto component : _EntityComponentIndex.at(entityID)->_components)
		component.lock()->SendComponentMessage(message);
}




//CreateComponents
//	Entity��Components���쐬
//
std::weak_ptr<ComponentList> ComponentManager::CreateComponents(IEntity* entity)
{
	return _EntityComponentIndex.emplace(entity->GetEntityID(), std::shared_ptr<ComponentList>(new ComponentList())).first->second;
}

//GetComponents
//	Entity�̎����Ă���Components���擾
//
std::weak_ptr<ComponentList> ComponentManager::GetComponents(IEntity* entity)
{
	return _EntityComponentIndex.at(entity->GetEntityID());
}

//DestroyComponents
//	Entity�̎����Ă���Components�� Destroy�֐��𗘗p���č폜
//
void ComponentManager::DestroyComponents(IEntity* entity)
{
	for(auto component : _EntityComponentIndex.at(entity->GetEntityID())->_components)
		component.lock()->Destroy();
}

//ReleaseComponents
//	Entity�̎����Ă���Components���J��
//
void ComponentManager::ReleaseComponents(IEntity* entity)
{
	_EntityComponentIndex.erase(entity->GetEntityID());
}





//ImGui_ComponentView
//	Entity�̎����Ă���Components��Debug�\��
//
void ComponentManager::ImGui_ComponentView(EntityID id)
{
	for (auto component : _EntityComponentIndex.at(id)->_components)
	{
		component.lock()->OnDebugImGui();
	}
}