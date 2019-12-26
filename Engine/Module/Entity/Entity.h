#pragma once

//Entity
//	
//
template<typename Type>
class Entity:public IEntity
{
protected:
	//����Entity�̃C���X�^���X�z��
	static std::unordered_map<EntityID,std::weak_ptr<Type>> g_EntityIndex;

public:
	//�R���X�g���N�^
	Entity();
	//�f�X�g���N�^
	virtual ~Entity();

	//Entity�̎擾
	static std::weak_ptr<Type> GetTypeEntity(EntityID id)
	{
		return g_EntityIndex.at(id);
	}
};

template<typename Type>
std::unordered_map<EntityID, std::weak_ptr<Type>> Entity<Type>::g_EntityIndex;



//Entity
//	�R���X�g���N�^
//
template<typename Type>
Entity<Type>::Entity()
{
	
}

//~Entity
//	�f�X�g���N�^
//
template<typename Type>
Entity<Type>::~Entity()
{
	g_EntityIndex.erase(GetEntityID());
}