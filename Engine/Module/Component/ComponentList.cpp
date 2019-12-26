#include"Common.h"

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Object\Object.h"
#include"Module\Object\ObjectManager.h"

#include"IComponent.h"
#include"ComponentList.h"

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

}

//Add
//	list�֒ǉ�
//
void ComponentList::Add(std::weak_ptr<IComponent> component)
{
	_components.push_back(component);
}

void ComponentList::Add(ComponentList * list)
{
	_components.insert(_components.end(),list->_components.begin(),list->_components.end());
}

//Get
//	list����Component���擾
//
std::weak_ptr<IComponent> ComponentList::Get(ComponentTypeID componentTypeID)
{
	for (auto obj : _components)
	{
		if (obj.lock()->GetComponentTypeID() == componentTypeID)
			return obj.lock();
	}
	return std::weak_ptr<IComponent>();
}

//Remove
//	list����ComponentTypeID����v������̂��폜
//
void ComponentList::Remove(ComponentTypeID componentTypeID)
{
	_components.remove_if(
		[componentTypeID](std::weak_ptr<IComponent> component)
	{
		return component.lock()->GetComponentTypeID() == componentTypeID;
	});
}

//Remove
//	list����ComponentID����v������̂��폜
//
void ComponentList::Remove(ComponentID componentID)
{
	_components.remove_if(
		[componentID](std::weak_ptr<IComponent> component)
	{
		return component.lock()->GetComponentID() == componentID;
	});
}

//Release
//	components���폜
//
void ComponentList::Release()
{
	_components.clear();
}
