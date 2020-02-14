#include"Common.h"

#include"Module\Object\Object.h"

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Component\IComponent.h"
#include"Module\Component\ComponentList.h"
#include"Module\Component\ComponentManager.h"

#include"IEntity.h"
#include"EntityManager.h"

//*********************************************************************************************************************
//
//	IEntity
//
//*********************************************************************************************************************

//IEntity
//	コンストラクタ
//
IEntity::IEntity()
	:
	Object(),
	//ComponentListの生成
	_ComponentList(ComponentManager::GetInstance()->CreateComponents(this))
{

}

//~IEntity
//	デストラクタ
//
IEntity::~IEntity()
{
	//ComponentListの解放
	ComponentManager::GetInstance()->ReleaseComponents(this);
}

//Destory
//	Entityの削除命令
//
void IEntity::Destroy()
{
	//ComponentListの削除登録
	ComponentManager::GetInstance()->DestroyComponents(this);
	ObjectManager::GetInstance()->DestroyObject(this);
}

//Release
//	即時破棄関数
//
void IEntity::Release()
{
	EntityManager::GetInstance()->ReleaseEntity(this);
}
