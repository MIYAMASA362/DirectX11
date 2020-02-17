#pragma once


using Components = std::vector<std::shared_ptr<IComponent>>;
//*********************************************************************************************************************
//
//	IEntity
//		Component��ێ�
//
//*********************************************************************************************************************
class IEntity :public Object
{
	friend cereal::access;
	friend class IComponent;
private:
	//Components
	Components _Components;


public:
	//�R���X�g���N�^
	IEntity();
	//�f�X�g���N�^
	virtual ~IEntity();

	//Entity�̎���ID
	inline EntityID GetEntityID();

	//Components�̎擾
	Components& GetComponents();

	//Component�̒ǉ�
	template<typename Type>
	std::shared_ptr<Type> AddComponent();

	//Component�̎擾
	template<typename Type>
	std::shared_ptr<Type> GetComponent();

	//�폜�錾
	void Destroy() override;
	//Components�ւ̑��M
	void SendComponentMessage(std::string message);

protected:
	//�����j���֐�
	virtual void Release() override;
	//Component�̔j��
	void ReleaseComponent(IComponent* component);
	//ObjectManager�o�^�����s�֐�
	virtual void Register(std::shared_ptr<Object> instance) override;


private:
	//�V���A���C�Y
	template<class Archive>
	void save(Archive& archive) const
	{
		cereal::base_class<Object>(this);
		archive(_Components);
	}

	//�f�V���A���C�Y
	template<class Archive>
	void load(Archive& archive)
	{
		cereal::base_class<Object>(this);
		archive(_Components);
	}


};

inline EntityID IEntity::GetEntityID()
{
	return GetInstanceID();
}

inline Components & IEntity::GetComponents()
{
	return _Components;
}


CEREAL_REGISTER_TYPE(IEntity)

template<typename Type>
inline std::shared_ptr<Type> IEntity::AddComponent()
{
	//Component�̐���
	auto component = std::shared_ptr<Type>(new Type());

	//OwnerEntity�̐ݒ�
	component->SetEntity(this);

	//ObjectManager�֓o�^
	ObjectManager::GetInstance()->RegisterObject(component);

	//Components�ւ̒ǉ�
	this->_Components.push_back(component);

	return component;
}

template<typename Type>
inline std::shared_ptr<Type> IEntity::GetComponent()
{
	for (auto component : this->_Components)
	{
		if (std::shared_ptr<Type> result = std::dynamic_pointer_cast<Type>(component))
			return result;
	}
	return std::shared_ptr<Type>();
}
