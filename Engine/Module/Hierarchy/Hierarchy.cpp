#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include"../ECSEngine.h"

#include "Hierarchy.h"

using namespace DirectX;

//*********************************************************************************************************************
//
//	Hierarchy
//
//*********************************************************************************************************************

//Hierarchy
//	コンストラクタ
//
Hierarchy::Hierarchy()
{

}

Hierarchy::Hierarchy(std::weak_ptr<IEntity> self)
	:
	_self(self)
{

}

//~Hierarchy
//	デストラクタ
//
Hierarchy::~Hierarchy()
{
	
}

//DetachParent
//
//
void Hierarchy::DetachParent()
{
	_parent.reset();
}

//AttachChild
//
//
void Hierarchy::AttachChild(std::weak_ptr<IEntity> child)
{
	_children.push_back(child);
}

//DetachChild
//
//
void Hierarchy::DetachChild(std::weak_ptr<IEntity> child)
{
	_children.remove_if([child](std::weak_ptr<IEntity> obj) { return obj.lock() == child.lock(); });
}

//DetachChildren
//
//
void Hierarchy::DetachChildren()
{
	_children.clear();
}




//*********************************************************************************************************************
//
//	HierarchyUtility
//
//*********************************************************************************************************************

//HierarchyUtility
//	コンストラクタ
//
HierarchyUtility::HierarchyUtility()
{
}


HierarchyUtility::~HierarchyUtility()
{

}

std::weak_ptr<IEntity> HierarchyUtility::GetParent(EntityID id)
{
	return _hierarchyMap.at(id)._parent;
}

std::list<std::weak_ptr<IEntity>> HierarchyUtility::GetAllParent(EntityID id)
{
	std::list<std::weak_ptr<IEntity>> list;
	auto parent = _hierarchyMap.at(id)._parent;
	if (parent.expired()) return list;
	list.push_back(parent);
	list.splice(list.end(),GetAllParent(parent.lock()->GetEntityID()));
	return list;
}

std::list<std::weak_ptr<IEntity>> HierarchyUtility::GetChildren(EntityID id)
{
	return _hierarchyMap.at(id)._children;
}

std::list<std::weak_ptr<IEntity>> HierarchyUtility::GetAllChildren(EntityID id)
{
	std::list<std::weak_ptr<IEntity>> list;
	auto children = _hierarchyMap.at(id)._children;
	if (children.size() == 0)return list;
	list.splice(list.end(),children);
	for(auto child :children)
		if(!child.expired())
			list.splice(list.end(),GetAllChildren(child.lock()->GetEntityID()));
	return list;
}

void HierarchyUtility::AttachHierarchy(std::shared_ptr<IEntity> entity)
{
	_hierarchyMap.emplace(entity->GetEntityID(),entity);
}

void HierarchyUtility::DetachHierarchy(EntityID id)
{
	_hierarchyMap.erase(id);
}

void HierarchyUtility::ClearnHierarchy()
{
	_hierarchyMap.clear();
}

void HierarchyUtility::AttachParent(EntityID id, std::weak_ptr<IEntity> parent)
{
	Hierarchy& hierarchy = _hierarchyMap.at(id);
	hierarchy.AttachParent(parent);
	_hierarchyMap.at(parent.lock()->GetEntityID()).AttachChild(hierarchy._self);
}

void HierarchyUtility::AttachParent(std::weak_ptr<IEntity> self, std::weak_ptr<IEntity> parent)
{
	AttachParent(self.lock()->GetEntityID(),parent);
}

void HierarchyUtility::DetachParent(EntityID id)
{
	Hierarchy& hierarchy = _hierarchyMap.at(id);
	hierarchy.DetachParent();
	_hierarchyMap.at(hierarchy._parent.lock()->GetEntityID()).DetachChild(hierarchy._self);
}

void HierarchyUtility::AttachChild(EntityID id, std::weak_ptr<IEntity> child)
{
	Hierarchy& hierarchy = _hierarchyMap.at(id);
	hierarchy.AttachChild(child);
	_hierarchyMap.at(child.lock()->GetEntityID()).AttachParent(hierarchy._self);
}

void HierarchyUtility::DetachChild(EntityID id, std::weak_ptr<IEntity> child)
{
	Hierarchy& hierarchy = _hierarchyMap.at(id);
	hierarchy.DetachChild(child);
	_hierarchyMap.at(child.lock()->GetEntityID()).DetachParent();
}

void HierarchyUtility::DetachChildren(EntityID id)
{
	Hierarchy& hierarchy = _hierarchyMap.at(id);
	for (auto child : hierarchy._children)
		_hierarchyMap.at(child.lock()->GetEntityID()).DetachParent();
	hierarchy.DetachChildren();
}


Hierarchy * HierarchyUtility::GetHierarchy(EntityID id)
{
	return &_hierarchyMap.at(id);
}

std::map<EntityID, Hierarchy> HierarchyUtility::GetHierarchyMap()
{
	return _hierarchyMap;
}

