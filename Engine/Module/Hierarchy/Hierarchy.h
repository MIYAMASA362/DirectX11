#pragma once


class HierarchyUtility;

class Hierarchy
{
	friend HierarchyUtility;
private:
	HierarchyUtility* _utility;
	std::weak_ptr<IEntity> _self;
	std::weak_ptr<IEntity> _parent;
	std::list<std::weak_ptr<IEntity>> _children;

public:
	Hierarchy(HierarchyUtility* utility,std::weak_ptr<IEntity> self);
	~Hierarchy();

	HierarchyUtility& GetUtility();
	std::weak_ptr<IEntity>& GetSelf();
	std::weak_ptr<IEntity>& GetParent();
	std::list<std::weak_ptr<IEntity>>& GetChildren();

private:
	void AttachParent(std::weak_ptr<IEntity> parent);
	void DetachParent();

	void AttachChild(std::weak_ptr<IEntity> child);
	void DetachChild(std::weak_ptr<IEntity> child);
	void DetachChildren();
};

class HierarchyUtility
{
private:
	std::map<EntityID, Hierarchy> _hierarchyMap;
public:
	std::weak_ptr<IEntity> GetParent(EntityID id);
	std::list<std::weak_ptr<IEntity>> GetAllParent(EntityID id);
	std::list<std::weak_ptr<IEntity>> GetChildren(EntityID id);
	std::list<std::weak_ptr<IEntity>> GetAllChildren(EntityID id);

	void AttachHierarchy(EntityID id);
	void DetachHierarchy(EntityID id);
	void ClearnHierarchy();

	void AttachParent(EntityID id, std::weak_ptr<IEntity> parent);
	void AttachParent(std::weak_ptr<IEntity> self, std::weak_ptr<IEntity> parent);
	void DetachParent(EntityID id);

	void AttachChild(EntityID id, std::weak_ptr<IEntity> child);
	void DetachChild(EntityID id,std::weak_ptr<IEntity> child);
	void DetachChildren(EntityID id);

	Hierarchy* GetHierarchy(EntityID id);
	std::map<EntityID, Hierarchy> GetHierarchyMap();
};
