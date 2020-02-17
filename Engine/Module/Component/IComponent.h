#pragma once


class Transform;
class GameObject;
class IEntity;

//*********************************************************************************************************************
//
//	IComponent
//		�f�[�^Component
//
//*********************************************************************************************************************
class IComponent:public Object
{
	friend cereal::access;
private:
	//OwnerEntity
	IEntity* _Entity;


public:
	//�R���X�g���N�^
	IComponent();
	//�f�X�g���N�^
	virtual ~IComponent();

	//ComponentID�̎擾
	ComponentID GetComponentID() { return GetInstanceID(); };
	//Owner�̐ݒ�
	void SetEntity(IEntity* owner) { _Entity = owner; };

	//GameObject
	GameObject* gameObject();
	//Message�𑗐M
	virtual void SendComponentMessage(std::string message) {};

	EntityID GetOwnerID();

protected:
	//ImGui�̐ݒ�
	virtual void OnDebugImGui();
	//�폜�����s�֐�
	virtual void Release() override;
	//ObjectManager�o�^�����s�֐�
	virtual void Register(std::shared_ptr<Object> instance) override;


private:
	//�V���A���C�Y
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Object>(this)
		);
	}

	//�f�V���A���C�Y
	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Object>(this)
		);
	}


};

CEREAL_REGISTER_TYPE(IComponent)