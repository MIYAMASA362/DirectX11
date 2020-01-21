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
//	�R���X�g���N�^
//
IEntity::IEntity()
	:
	Object()
{

}

IEntity::IEntity(std::shared_ptr<IEntity> sptr, std::shared_ptr<ComponentList> components)
	:
	Object(),
	_self(sptr),
	_components(components)
{

}

//~IEntity
//	�f�X�g���N�^
//
IEntity::~IEntity()
{
	_self.reset();
	_components.reset();
}

//Destroy
//	Object�̍폜
//
void IEntity::Destroy()
{
	this->DestroyComponents();
	Object::Destroy();
}
