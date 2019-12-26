#include"Common.h"

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Object\Object.h"
#include"Module\Object\ObjectManager.h"

#include"Module\Component\IComponent.h"
#include"Module\Component\ComponentList.h"
#include"Module\Component\ComponentManager.h"

#include"Module\Entity\IEntity.h"
#include"Module\Entity\EntityManager.h"

//IEntity
//	コンストラクタ
//
IEntity::IEntity()
{
	_self = EntityManager::CreateEntity(this);
	_components = ComponentManager::CreateComponents(this);
}

//~IEntity
//	デストラクタ
//
IEntity::~IEntity()
{
	EntityManager::ReleaseEntity(this);
}



//DestroyComponents
//	ComponentManagerからComponentsを削除する
//
void IEntity::DestroyComponents()
{
	ComponentManager::DestroyComponents(this);
}

//Destroy
//	Objectの削除
//
void IEntity::Destroy()
{
	this->DestroyComponents();
	Object::Destroy();
}
