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
	friend cereal::access;
private:
	//���g
	std::weak_ptr<IEntity> _self;
	//�e
	std::weak_ptr<IEntity> _parent;
	//�q
	std::list<std::weak_ptr<IEntity>> _children;

	

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(_self),
			CEREAL_NVP(_parent),
			CEREAL_NVP(_children)
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(_self),
			CEREAL_NVP(_parent),
			CEREAL_NVP(_children)
		);
	}

public:
	//�R���X�g���N�^
	Hierarchy();
	//�R���X�g���N�^
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
class HierarchyUtility final
{
	friend cereal::access;
private:
	//�I�u�W�F�N�g���̊K�w
	std::map<EntityID, Hierarchy> _hierarchyMap;

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(_hierarchyMap)
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(_hierarchyMap)
		);
	}


public:
	//�R���X�g���N�^
	HierarchyUtility();
	//�f�X�g���N�^
	~HierarchyUtility();

	//�e�̎擾
	std::weak_ptr<IEntity> GetParent(EntityID id);
	//
	std::list<std::weak_ptr<IEntity>> GetAllParent(EntityID id);
	//�q�̎擾
	std::list<std::weak_ptr<IEntity>> GetChildren(EntityID id);
	//�S�Ă̎q���擾
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
