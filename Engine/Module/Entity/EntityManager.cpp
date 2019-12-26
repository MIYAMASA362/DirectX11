#include"Common.h"
#include<functional>

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Object\Object.h"
#include"Module\Component\IComponent.h"
#include"Module\Component\ComponentList.h"
#include"Module\Component\ComponentManager.h"

#include"Module\Entity\IEntity.h"

#include"EntityManager.h"

//pInstance
//	Singleton インスタンス
//
EntityManager* EntityManager::g_pInstacne = nullptr;



//EntityManager
//	コンストラクタ
//	EntityIndexの生成
//
EntityManager::EntityManager()
{
	_EntityIndex = new EntityIndex();
}

//~EntityManager
//	デストラクタ
//	EntityIndexの破棄
//
EntityManager::~EntityManager()
{
	_EntityIndex->clear();
	delete _EntityIndex;
}



//Create
//	インスタンス生成
//
void EntityManager::Create()
{
	if (g_pInstacne) return;
	g_pInstacne = new EntityManager();
}

//Release
//	インスタンス破棄
//
void EntityManager::Release()
{
	if (g_pInstacne == nullptr) return;
	delete g_pInstacne;
	g_pInstacne = nullptr;
}



//CreateEntity
//	EntityIndexへの追加
//
std::weak_ptr<IEntity> EntityManager::CreateEntity(IEntity* instance)
{
	auto sptr = std::dynamic_pointer_cast<IEntity>(instance->Object::_self.lock());
	return g_pInstacne->_EntityIndex->emplace(instance->GetEntityID(), sptr).first->second;
}

//GetEntity
//	EntityIndexからEntityの取得
//
std::weak_ptr<IEntity> EntityManager::GetEntity(EntityID id)
{
	auto find = g_pInstacne->_EntityIndex->find(id);
	if (find == g_pInstacne->_EntityIndex->end()) 
		return std::weak_ptr<IEntity>();
	return find->second;
}

//ReleaseEntity
//	EntityIndexから削除する
//
void EntityManager::ReleaseEntity(IEntity * instance)
{
	g_pInstacne->_EntityIndex->erase(instance->GetEntityID());
}