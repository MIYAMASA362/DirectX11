#pragma once

//*********************************************************************************************************************
//
//	IEntity
//
//*********************************************************************************************************************
class IEntity :public Object
{
	friend class EntityManager;
	friend cereal::access;
private:
	//EntityManager�ŊǗ�����Ă���Instance�ւ̃A�N�Z�X
	std::weak_ptr<IEntity> _self;
	//ComponentManager�ŊǗ�����Ă���Components�ւ̃A�N�Z�X
	std::weak_ptr<ComponentList> _components;

	template<class Archive>
	void save(Archive& archive)const
	{
		cereal::base_class<Object>(this);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		cereal::base_class<Object>(this);
	}

public:
	//�R���X�g���N�^
	IEntity();
	IEntity(std::shared_ptr<IEntity> sptr,std::shared_ptr<ComponentList> components);

	//�f�X�g���N�^
	virtual ~IEntity();

	//Entity�̎���ID
	EntityID GetEntityID() { return GetInstanceID();};

	//Entity�̃|�C���^�擾
	std::weak_ptr<IEntity> GetEntity() { return _self;};

	//Components�ւ̒ǉ�
	template<typename Type> 
	std::weak_ptr<Type> AddComponent() { return ComponentManager::GetInstance()->AddComponent<Type>(this);}

	//Components����Component�̎擾
	template<typename Type>
	std::weak_ptr<Type> GetComponent() { return ComponentManager::GetInstance()->GetComponent<Type>(this); }

	//Components�̎擾
	std::shared_ptr<ComponentList> GetComponents() { return _components.lock();}

	//Components����Component�̍폜
	template<typename Type> 
	void DestroyComponent() { ComponentManager::GetInstance()->DestroyComponent<Type>(this); }

	//Components�̍폜
	void DestroyComponents() { ComponentManager::GetInstance()->DestroyComponents(this); }

	//ObjectManager��ʂ����j��
	void Destroy() override;


};

CEREAL_REGISTER_TYPE(IEntity)