#include"Common.h"

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Object\Object.h"
#include"Module\Object\ObjectManager.h"

#include"IComponent.h"
#include"ComponentList.h"

//*********************************************************************************************************************
//
//	ComponentList
//
//*********************************************************************************************************************

//ComponentList
//	�R���X�g���N�^
//
ComponentList::ComponentList()
{

}

//~ComponentList
//	�f�X�g���N�^
//
ComponentList::~ComponentList()
{
	//Components�̔j��
	_Components.clear();
}

//AddComponent
//	Components��Component��ǉ�����
//
void ComponentList::AddComponent(std::shared_ptr<IComponent> add)
{
	_Components.emplace(add->GetComponentID(),add);
}

std::shared_ptr<IComponent>& ComponentList::GetComponent(ComponentTypeID id)
{
	for(auto component : _Components)
	{
		if (component.second->GetComponentTypeID() == id)
			return component.second;
	}
	assert(0);
	return std::shared_ptr<IComponent>();
}

//ReleaseComponent
//	Component�̏��L���j��
//
void ComponentList::ReleaseComponent(ComponentID componentID)
{
	_Components.erase(componentID);
}
