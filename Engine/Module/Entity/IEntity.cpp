#include<algorithm>
#include"Common.h"

#include"Module\Object\Object.h"

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Component\IComponent.h"
#include"Module\Component\Component.h"
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
	_Components.reserve(COMPONENTS_CAPACITY);
}

//~IEntity
//	�f�X�g���N�^
//
IEntity::~IEntity()
{
	_Components.clear();
}

//Destroy
//	�폜�錾
//
void IEntity::Destroy()
{
	Object::Destroy();
	for(auto component : _Components)
	{
		component->Destroy();
	}
}

void IEntity::SendComponentMessage(std::string message)
{
	for (auto component : _Components)
	{
		component->SendComponentMessage(message);
	}
}

//Release
//	�����j���֐�
//
void IEntity::Release()
{
	EntityManager::GetInstance()->ReleaseEntity(this);
}

//ReleaseComponent
//	�R���|�[�l���g�̔j��
//
void IEntity::ReleaseComponent(IComponent * component)
{
	ComponentID id = component->GetComponentID();

	auto end = _Components.end();
	auto find = std::remove_if(
		_Components.begin(), end, [id](std::shared_ptr<IComponent> component)
	{
		return component->GetComponentID() == id;
	});
	_Components.erase(find,end);
}

void IEntity::Register(std::shared_ptr<Object> instance)
{
	Object::Register(instance);
	EntityManager::GetInstance()->RegisterEntity(std::dynamic_pointer_cast<IEntity>(instance));
}
