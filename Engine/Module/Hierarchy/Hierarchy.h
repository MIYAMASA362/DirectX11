#pragma once


class HierarchyUtility;

//*********************************************************************************************************************
//
//	Hierarchy
//
//*********************************************************************************************************************
class Hierarchy
{
	friend HierarchyUtility;
private:
	//自身
	std::weak_ptr<IEntity> _self;
	//親
	std::weak_ptr<IEntity> _parent;
	//子
	std::list<std::weak_ptr<IEntity>> _children;


public:
	Hierarchy(std::weak_ptr<IEntity> self);
	~Hierarchy();

	std::weak_ptr<IEntity>& GetSelf() { return _self; }
	std::weak_ptr<IEntity>& GetParent() { return _parent; }
	std::list<std::weak_ptr<IEntity>>& GetChildren() { return _children; }


private:
	void AttachParent(std::weak_ptr<IEntity> parent) { _parent = parent; };
	void DetachParent();

	void AttachChild(std::weak_ptr<IEntity> child);
	void DetachChild(std::weak_ptr<IEntity> child);
	void DetachChildren();
	

};

//*********************************************************************************************************************
//
//	HierarchyUtility
//
//*********************************************************************************************************************
class HierarchyUtility
{
private:
	//オブジェクト毎の階層
	std::map<EntityID, Hierarchy> _hierarchyMap;


public:
	//親の取得
	std::weak_ptr<IEntity> GetParent(EntityID id);
	//
	std::list<std::weak_ptr<IEntity>> GetAllParent(EntityID id);
	//子の取得
	std::list<std::weak_ptr<IEntity>> GetChildren(EntityID id);
	//全ての子を取得
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
