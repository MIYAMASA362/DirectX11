#include"Common.h"

#include"Module\Object\Object.h"
#include"Module\Object\ObjectManager.h"

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Component\IComponent.h"
#include"Module\Component\ComponentList.h"
#include"Module\Component\ComponentManager.h"

#include"IEntity.h"
#include"EntityManager.h"


//*********************************************************************************************************************
//
//	EntityManager
//
//*********************************************************************************************************************

//pInstance
//	Singleton �C���X�^���X
//
EntityManager* EntityManager::pInstacne = nullptr;



//EntityManager
//	�R���X�g���N�^
//	EntityIndex�̐���
//
EntityManager::EntityManager()
{
	
}

//~EntityManager
//	�f�X�g���N�^
//	EntityIndex�̔j��
//
EntityManager::~EntityManager()
{
	_EntityIndex.clear();
}



//Create
//	�C���X�^���X����
//
void EntityManager::Create()
{
	if (pInstacne != nullptr) return;
	pInstacne = new EntityManager();
}

//Release
//	�C���X�^���X�j��
//
void EntityManager::Release()
{
	if (pInstacne == nullptr) return;
	delete pInstacne;
	pInstacne = nullptr;
}



//GetEntity
//	EntityIndex����Entity�̎擾
//
std::weak_ptr<IEntity> EntityManager::GetEntity(EntityID id)
{
	auto find = _EntityIndex.find(id);
	if (find == _EntityIndex.end()) assert(0);
	return find->second;
}

//RegisterEntity
//	EntityIndex�ւ̒ǉ�
//
std::weak_ptr<IEntity> EntityManager::RegisterEntity(IEntity * instance)
{
	if (instance->Object::GetSelf().expired()) assert(0);

	auto result = std::dynamic_pointer_cast<IEntity>(instance->GetSelf().lock());
	_EntityIndex.emplace(result->GetEntityID(),result);

	instance->_self = result;
	instance->_components = ComponentManager::GetInstance()->CreateComponents(result.get());

	return result;
}

void EntityManager::DestroyEntity(IEntity * instance)
{
	_EntityIndex.erase(instance->GetEntityID());
}

//ReleaseEntity
//	EntityIndex����폜����
//
void EntityManager::ReleaseEntity(IEntity * instance)
{
	_EntityIndex.erase(instance->GetEntityID());
}


