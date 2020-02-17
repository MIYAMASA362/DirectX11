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
//	Singleton インスタンス
//
EntityManager* EntityManager::pInstacne = nullptr;



//EntityManager
//	コンストラクタ
//	EntityIndexの生成
//
EntityManager::EntityManager()
	:
	_EntityIndex(ENTITY_CAPACITY)
{

}

//~EntityManager
//	デストラクタ
//	EntityIndexの破棄
//
EntityManager::~EntityManager()
{
	_EntityIndex.clear();
}



//Create
//	インスタンス生成
//
void EntityManager::Create()
{
	if (pInstacne != nullptr) return;
	pInstacne = new EntityManager();
}

//Release
//	インスタンス破棄
//
void EntityManager::Release()
{
	if (pInstacne == nullptr) return;
	delete pInstacne;
	pInstacne = nullptr;
}


//RegisterEntity
//	EntityIndexへの追加
//
void EntityManager::RegisterEntity(std::shared_ptr<IEntity> instance)
{
	_EntityIndex.push_back(instance);
}

//ReleaseEntity
//	EntityIndexから削除する
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


