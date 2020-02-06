#pragma once

//*********************************************************************************************************************
//
//	ComponentArray
//	�@IComponent�̔z��
//
//*********************************************************************************************************************
class ComponentList final
{
	friend class ComponentManager;
	friend cereal::access;
public:
	using Components = std::unordered_map<ComponentID, std::shared_ptr<IComponent>>;
private:

	//Components
	Components _Components;

	//�V���A����
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(_Components);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(_Components);
	}

public:

	//�R���X�g���N�^
	ComponentList();
	//�f�X�g���N�^
	~ComponentList();
	
	//GetComponents Components�̎擾
	Components& GetComponents()
	{
		return _Components;
	}

	//Components�֒ǉ�
	void AddComponent(std::shared_ptr<IComponent> add);
	//Component�̎擾 ��ԍŏ��̕�
	std::shared_ptr<IComponent>& GetComponent(ComponentTypeID id);
	//Component�̏��L��j��
	void ReleaseComponent(ComponentID componentID);
};
