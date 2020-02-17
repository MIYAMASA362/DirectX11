#pragma once

//*********************************************************************************************************************
//
//	Entity
//		Type����Entity�Ǘ�
//
//*********************************************************************************************************************
template<typename Type>
class Entity:public IEntity
{
	friend cereal::access;
protected:
	//����Entity�̃C���X�^���X�z��
	static std::list<std::weak_ptr<Type>> EntityIndex;


public:
	//�R���X�g���N�^
	Entity();
	//�f�X�g���N�^
	virtual ~Entity();

	//EntityIndex�̎擾
	static std::list<std::weak_ptr<Type>>& GetEntityIndex() { return EntityIndex; };

	

protected:
	//�����j���֐�
	virtual void Release() override;
	//ObjectManager�o�^�����s�֐�
	virtual void Register(std::shared_ptr<Object> instance) override;

	//EntityIndex����폜
	static void ReleaseEntityIndex(Type* instance);
	//EntityIndex�֓o�^
	static void RegisterEntityIndex(std::shared_ptr<Type> instance);


private:
	//�V���A���C�Y
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::base_class<IEntity>(this));
	}

	//�f�V���A���C�Y
	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::base_class<IEntity>(this));
	}


};



template<typename Type>
std::list<std::weak_ptr<Type>> Entity<Type>::EntityIndex;

//Entity
//	�R���X�g���N�^
//
template<typename Type>
Entity<Type>::Entity()
	:
	IEntity()
{
	
}

//~Entity
//	�f�X�g���N�^
//
template<typename Type>
Entity<Type>::~Entity()
{
	
}

//RegisterEntityIndex
//	
//
template<typename Type>
inline void Entity<Type>::RegisterEntityIndex(std::shared_ptr<Type> instance)
{
	return EntityIndex.push_back(instance);
}

//ReleaseEntityIndex
//
//
template<typename Type>
inline void Entity<Type>::ReleaseEntityIndex(Type* instance)
{
	EntityID id = instance->GetEntityID();

	auto end = EntityIndex.end();
	auto find = std::remove_if(
		EntityIndex.begin(), end, [id](std::weak_ptr<Type> entity) 
	{
		return entity.lock()->GetEntityID() == id;
	});
	EntityIndex.erase(find,end);
}

//Release
//
//
template<typename Type>
inline void Entity<Type>::Release()
{
	IEntity::Release();
	ReleaseEntityIndex(static_cast<Type*>(this));
}

template<typename Type>
inline void Entity<Type>::Register(std::shared_ptr<Object> instance)
{
	IEntity::Register(instance);
	this->RegisterEntityIndex(std::dynamic_pointer_cast<Type>(instance));
}
