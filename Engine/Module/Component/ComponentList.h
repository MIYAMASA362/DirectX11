#pragma once

using Components = std::list<std::shared_ptr<IComponent>>;

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
private:

	//Components
	Components _Components;

public:

	//�R���X�g���N�^
	ComponentList();
	//�f�X�g���N�^
	~ComponentList();
	
	//GetComponents Components�̎擾
	inline Components& GetComponents()
	{
		return _Components;
	}

private:
	//�V���A���C�Y
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(_Components);
	}

	//�f�V���A���C�Y
	template<class Archive>
	void load(Archive& archive)
	{
		archive(_Components);
	}

};
