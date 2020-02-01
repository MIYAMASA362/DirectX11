#pragma once

class GameObject;
class Hierarchy;

namespace cereal
{
	class access;
}

//*********************************************************************************************************************
//
//	Transform
//
//*********************************************************************************************************************
class Transform final : public Component<Transform>
{
	friend cereal::access;
private:
	//�e�q
	Hierarchy*  _hierarchy;

	//�ʒu
	Vector3	_Position;
	//��]
	Quaternion _Rotation;
	//�X�P�[��
	Vector3 _Scale;


	//�R���X�g���N�^
	Transform();

public:
	//�R���X�g���N�^
	Transform(EntityID OwnerID);
	//�f�X�g���N�^
	~Transform();

	//�ʒu �擾
	Vector3	position();			//���[���h
	Vector3	localPosition();	//���[�J��

	//��] �擾
	Quaternion rotation();		//���[���h
	Quaternion localRotation(); //���[�J��

	//�X�P�[�� �擾
	Vector3	scale();			//���[���h
	Vector3	localScale();		//���[�J��

	//�ʒu �ݒ�
	void position(Vector3 position);					//���[���h�ʒu
	void localPosition(Vector3 position);				//���[�J���ʒu

	//��] �ݒ�
	void rotation(Quaternion rotation);					//���[���h��]
	void localRotation(Quaternion rotation);			//���[�J����]

	//�X�P�[���ݒ�
	void localScale(Vector3 scale);						//���[�J���傫��

	//�e�q�֌W
	void SetParent(std::weak_ptr<Transform> pParent);	//�e�q��ݒ�
	void SetParent(std::weak_ptr<GameObject> parent);
	void DetachParent();								//�e�𗣂�
	void DetachChildren();								//�q�𗣂�
	std::weak_ptr<Transform> GetParent();				//�e�擾
	std::list<std::weak_ptr<Transform>> GetChildren();	//�q�擾

	void SendComponentMessageChildren(std::string message);	//���g�Ǝq��Components��message�𑗂�
	std::weak_ptr<IComponent> GetComponentInParent(ComponentTypeID componentTypeID);
	std::weak_ptr<IComponent> GetComponentInChildren(ComponentTypeID componentTypeID);
	ComponentList GetComponentsInChildren(ComponentTypeID componentTypeID);

	Vector3 right();
	Vector3 left();
	Vector3 up();
	Vector3 down();
	Vector3 forward();
	Vector3 back();

	XMMATRIX MatrixQuaternion();		//��]�s��
	XMMATRIX MatrixTranslation();		//�ړ��s��
	XMMATRIX MatrixScaling();			//�X�P�[���s��
	XMMATRIX WorldMatrix();				//���[���h�s��

	void LookAt(std::weak_ptr<Transform> target);	//���̕���������
	void OnDestroy() override;

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Component<Transform>>(this),
			CEREAL_NVP(_Position),
			CEREAL_NVP(_Rotation),
			CEREAL_NVP(_Scale)
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Component<Transform>>(this),
			CEREAL_NVP(_Position),
			CEREAL_NVP(_Rotation),
			CEREAL_NVP(_Scale)
		);
	}

protected:
	virtual IComponent* Internal_CreateInstance(IEntity* owner) override;

	void detachParent();								//�e�𗣂�
	void detachChild(std::weak_ptr<Transform> child);	//�e���^�[�Q�b�g�̎q��������ƍ폜����
	void childTransformUpdate();						//�q�̍s��ɕύX��������

	Vector3 TransformDirection(Vector3 direction);	//��]�s����g����Direction��ϊ�

	void OnDebugImGui() override;
};

CEREAL_REGISTER_TYPE(Component<Transform>)
CEREAL_REGISTER_TYPE(Transform)