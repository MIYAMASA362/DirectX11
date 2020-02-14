#pragma once


class Transform;
class GameObject;
class IEntity;

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
	//���L��
	std::shared_ptr<IEntity> _Entity;

	//�V���A���C�Y
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Object>(this),
			_Entity
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Object>(this),
			_Entity
		);
	}

public:
	//�R���X�g���N�^
	IComponent();
	IComponent(EntityID OwnerID);

	//�f�X�g���N�^
	virtual ~IComponent();

	//ComponentTypeID�̎擾
	ComponentTypeID GetComponentTypeID();
	//ComponentID�̎擾
	ComponentID GetComponentID() { return GetInstanceID(); };

	EntityID GetOwnerID();

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

	//�폜����
	virtual void Destroy();

	//�폜�����s�֐�
	virtual void Release();
};

CEREAL_REGISTER_TYPE(IComponent)