#include<algorithm>
#include"Common.h"

#include"Module\Object\Object.h"
#include"Module\Object\ObjectManager.h"

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Component\IComponent.h"
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
	:
	_EntityIndex(ENTITY_CAPACITY)
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


//RegisterEntity
//	EntityIndex�ւ̒ǉ�
//
void EntityManager::RegisterEntity(std::shared_ptr<IEntity> instance)
{
	_EntityIndex.push_back(instance);
}

//ReleaseEntity
//	EntityIndex����폜����
//
void EntityManager::ReleaseEntity(IEntity * instance)
{
	auto end = _EntityIndex.end();
	auto find = std::remove_if(
		_EntityIndex.begin(), end, [=](std::weak_ptr<IEntity>& entity) 
	{
		if (entity.expired()) return false;
		return entity.lock()->GetEntityID() == instance->GetEntityID();
	});
	_EntityIndex.erase(find,end);
}


