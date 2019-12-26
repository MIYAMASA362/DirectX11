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
//	�R���X�g���N�^
//
IEntity::IEntity()
{
	_self = EntityManager::CreateEntity(this);
	_components = ComponentManager::CreateComponents(this);
}

//~IEntity
//	�f�X�g���N�^
//
IEntity::~IEntity()
{
	EntityManager::ReleaseEntity(this);
}



//DestroyComponents
//	ComponentManager����Components���폜����
//
void IEntity::DestroyComponents()
{
	ComponentManager::DestroyComponents(this);
}

//Destroy
//	Object�̍폜
//
void IEntity::Destroy()
{
	this->DestroyComponents();
	Object::Destroy();
}
