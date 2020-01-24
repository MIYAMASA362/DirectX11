#pragma once


class Transform;
class GameObject;

//*********************************************************************************************************************
//
//	IComponent
//
//*********************************************************************************************************************
class IComponent:public Object
{
	//Component�̊Ǘ�
	friend class ComponentManager;
	friend cereal::access;
private:
	//OwnerID ���LEntity��ID
	EntityID _ownerId;

	//ComponentManager�ŊǗ�����Ă���Component
	std::weak_ptr<IComponent> _self;

	std::weak_ptr<Transform> _transform;
	std::weak_ptr<GameObject> _gameObject;

	//�V���A���C�Y
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Object>(this),
			CEREAL_NVP(_ownerId)
		);
	}
	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Object>(this),
			_ownerId
		);
	}

public:
	//�R���X�g���N�^
	IComponent();
	IComponent(EntityID OwnerID);
	//�f�X�g���N�^
	virtual ~IComponent();

	//�R���|�[�l���g�̎擾
	std::weak_ptr<IComponent> GetComponent() { return _self; };
	//OwnerID�̎擾
	EntityID GetOwnerID() const { return _ownerId; };
	//ComponentTypeID�̎擾
	ComponentTypeID GetComponentTypeID() { return typeid(*this).hash_code(); }
	//ComponentID�̎擾
	ComponentID GetComponentID() { return GetInstanceID(); };

	//Entity��Tranform�ւ̃A�N�Z�X
	std::shared_ptr<Transform> transform();
	//Entity��GameObject�ւ̃A�N�Z�X
	std::shared_ptr<GameObject> gameObject();


protected:
	//�폜�����s�֐�
	virtual void OnDestroy() {};

	//Component�Ƀ��b�Z�[�W�𑗐M���ꂽ���̓���
	virtual void SendComponentMessage(std::string message) {};

	//ImGui�̐ݒ�
	virtual void OnDebugImGui();


};

CEREAL_REGISTER_TYPE(IComponent)