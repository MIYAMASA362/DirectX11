#pragma once
namespace DirectX
{
	class GameObject;
	class Hierarchy;

	//�ϊ�Component
	class Transform :public Component<Transform>
	{
	private:
		Hierarchy*  _hierarchy;		//�e�q
	protected:
		Vector3		m_Position;		//�ʒu
		Quaternion	m_Rotation;		//��]
		Vector3		m_Scale;		//�T�C�Y
	public:
		Transform(EntityID OwnerID);
		~Transform() = default;

		Vector3		position();								//���[���h�ʒu
		Quaternion	rotation();								//���[���h��]
		Vector3		scale();								//���[���h�傫��

		Vector3		localPosition();						//���[�J���ʒu
		Quaternion	localRotation();						//���[�J����]
		Vector3		localScale();							//���[�J���傫��

		void position(Vector3 position);					//���[���h�ʒu
		void rotation(Quaternion rotation);					//���[���h��]

		void localPosition(Vector3 position);				//���[�J���ʒu
		void localRotation(Quaternion rotation);			//���[�J����]
		void localScale(Vector3 scale);						//���[�J���傫��
	protected:
		void detachParent();								//�e�𗣂�
		void detachChild(std::weak_ptr<Transform> child);	//�e���^�[�Q�b�g�̎q��������ƍ폜����
		void childTransformUpdate();						//�q�̍s��ɕύX��������
	public:
		void SetParent(std::weak_ptr<Transform> pParent);	//�e�q��ݒ�
		void SetParent(std::weak_ptr<GameObject> parent);
		void DetachParent();								//�e�𗣂�
		void DetachChildren();								//�q�𗣂�
		std::weak_ptr<Transform> GetParent();				//�e�擾
		std::list<std::weak_ptr<Transform>> GetChildren();	//�q�擾

		void SendComponentMessageChildren(std::string message);	//���g�Ǝq��Components��message�𑗂�
		std::weak_ptr<IComponent> GetComponentInParent(ComponentTypeID componentTypeID);
		std::weak_ptr<IComponent> GetComponentInChildren(ComponentTypeID componentTypeID);
		Components GetComponentsInChildren(ComponentTypeID componentTypeID);
	protected:
		Vector3 TransformDirection(Vector3 direction);	//��]�s����g����Direction��ϊ�
	public:
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
	};
}