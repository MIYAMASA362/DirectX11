#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include"../ECSEngine.h"

#include "Hierarchy.h"

using namespace DirectX;

DirectX::Hierarchy::Hierarchy(HierarchyUtility* utility,std::weak_ptr<IEntity> self)
	:
	_self(self),
	_utility(utility)
{

}

DirectX::Hierarchy::~Hierarchy()
{
	_utility = nullptr;
}

HierarchyUtility& DirectX::Hierarchy::GetUtility()
{
	return *_utility;
}

std::weak_ptr<IEntity>& DirectX::Hierarchy::GetSelf()
{
	return _self;
}

std::weak_ptr<IEntity>& DirectX::Hierarchy::GetParent()
{
	return _parent;
}

std::list<std::weak_ptr<IEntity>>& DirectX::Hierarchy::GetChildren()
{
	return _children;
}

void DirectX::Hierarchy::AttachParent(std::weak_ptr<IEntity> parent)
{
	_parent = parent;
}

void DirectX::Hierarchy::DetachParent()
{
	_parent.reset();
}

void DirectX::Hierarchy::AttachChild(std::weak_ptr<IEntity> child)
{
	_children.push_back(child);
}

void DirectX::Hierarchy::DetachChild(std::weak_ptr<IEntity> child)
{
	_children.remove_if([child](std::weak_ptr<IEntity> obj) { return obj.lock() == child.lock(); });
}

void DirectX::Hierarchy::DetachChildren()
{
	_children.clear();
}




std::weak_ptr<IEntity> DirectX::HierarchyUtility::GetParent(EntityID id)
{
	return _hierarchyMap.at(id)._parent;
}

std::list<std::weak_ptr<IEntity>> DirectX::HierarchyUtility::GetAllParent(EntityID id)
{
	std::list<std::weak_ptr<IEntity>> list;
	auto parent = _hierarchyMap.at(id)._parent;
	if (parent.expired()) return list;
	list.push_back(parent);
	list.splice(list.end(),GetAllParent(parent.lock()->GetEntityID()));
	return list;
}

std::list<std::weak_ptr<IEntity>> DirectX::HierarchyUtility::GetChildren(EntityID id)
{
	return _hierarchyMap.at(id)._children;
}

std::list<std::weak_ptr<IEntity>> DirectX::HierarchyUtility::GetAllChildren(EntityID id)
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

void DirectX::HierarchyUtility::AttachHierarchy(EntityID id)
{
	_hierarchyMap.emplace(id,Hierarchy(this,EntityManager::GetEntity(id)));
}

void DirectX::HierarchyUtility::DetachHierarchy(EntityID id)
{
	_hierarchyMap.erase(id);
}

void DirectX::HierarchyUtility::ClearnHierarchy()
{
	_hierarchyMap.clear();
}

void DirectX::HierarchyUtility::AttachParent(EntityID id, std::weak_ptr<IEntity> parent)
{
	Hierarchy& hierarchy = _hierarchyMap.at(id);
	hierarchy.AttachParent(parent);
	_hierarchyMap.at(parent.lock()->GetEntityID()).AttachChild(hierarchy._self);
}

void DirectX::HierarchyUtility::AttachParent(std::weak_ptr<IEntity> self, std::weak_ptr<IEntity> parent)
{
	AttachParent(self.lock()->GetEntityID(),parent);
}

void DirectX::HierarchyUtility::DetachParent(EntityID id)
{
	Hierarchy& hierarchy = _hierarchyMap.at(id);
	hierarchy.DetachParent();
	_hierarchyMap.at(hierarchy._parent.lock()->GetEntityID()).DetachChild(hierarchy._self);
}

void DirectX::HierarchyUtility::AttachChild(EntityID id, std::weak_ptr<IEntity> child)
{
	Hierarchy& hierarchy = _hierarchyMap.at(id);
	hierarchy.AttachChild(child);
	_hierarchyMap.at(child.lock()->GetEntityID()).AttachParent(hierarchy._self);
}

void DirectX::HierarchyUtility::DetachChild(EntityID id, std::weak_ptr<IEntity> child)
{
	Hierarchy& hierarchy = _hierarchyMap.at(id);
	hierarchy.DetachChild(child);
	_hierarchyMap.at(child.lock()->GetEntityID()).DetachParent();
}

void DirectX::HierarchyUtility::DetachChildren(EntityID id)
{
	Hierarchy& hierarchy = _hierarchyMap.at(id);
	for (auto child : hierarchy._children)
		_hierarchyMap.at(child.lock()->GetEntityID()).DetachParent();
	hierarchy.DetachChildren();
}


Hierarchy * DirectX::HierarchyUtility::GetHierarchy(EntityID id)
{
	return &_hierarchyMap.at(id);
}

std::map<EntityID, Hierarchy> DirectX::HierarchyUtility::GetHierarchyMap()
{
	return _hierarchyMap;
}

