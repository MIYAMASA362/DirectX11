#pragma once

//*********************************************************************************************************************
//
//	IEntity
//
//*********************************************************************************************************************
class IEntity :public Object
{
	friend cereal::access;
	friend class ComponentManager;
private:
	//Components �������Ă���Component�Q�ւ̎Q��
	std::weak_ptr<ComponentList> _ComponentList;

public:
	//�R���X�g���N�^
	IEntity();
	//�f�X�g���N�^
	virtual ~IEntity();

	//Entity�̎���ID
	EntityID GetEntityID() 
	{ 
		return GetInstanceID(); 
	}
	//Components�̎擾
	std::shared_ptr<ComponentList> GetComponents() 
	{
		return _ComponentList.lock();
	}

	//Component�̒ǉ�
	template<typename Type> 
	std::weak_ptr<Type> AddComponent() 
	{
		return ComponentManager::GetInstance()->AddComponent<Type>(this);
	}
	//Component�̎擾
	template<typename Type>
	std::weak_ptr<Type> GetComponent() 
	{
		return ComponentManager::GetInstance()->GetComponent<Type>(this);
	}

	//�폜�֐�
	void Destroy() override;

protected:
	//�����j���֐�
	virtual void Release() override;

private:
	//�V���A���C�Y
	template<class Archive>
	void save(Archive& archive) const
	{
		cereal::base_class<Object>(this);
		archive(_ComponentList.lock());
	}

	//�f�V���A���C�Y
	template<class Archive>
	void load(Archive& archive)
	{
		std::shared_ptr<ComponentList> list;
		cereal::base_class<Object>(this);
		archive(list);
		_ComponentList = ComponentManager::GetInstance()->SwapComponents(this, list);
	}

};

CEREAL_REGISTER_TYPE(IEntity)