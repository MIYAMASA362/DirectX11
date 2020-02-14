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
	Object(),
	//ComponentList�̐���
	_ComponentList(ComponentManager::GetInstance()->CreateComponents(this))
{

}

//~IEntity
//	�f�X�g���N�^
//
IEntity::~IEntity()
{
	//ComponentList�̉��
	ComponentManager::GetInstance()->ReleaseComponents(this);
}

//Destory
//	Entity�̍폜����
//
void IEntity::Destroy()
{
	//ComponentList�̍폜�o�^
	ComponentManager::GetInstance()->DestroyComponents(this);
	ObjectManager::GetInstance()->DestroyObject(this);
}

//Release
//	�����j���֐�
//
void IEntity::Release()
{
	EntityManager::GetInstance()->ReleaseEntity(this);
}
