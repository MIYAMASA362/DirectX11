#pragma once

//MonoBehaviour
//	����������ǉ�
//
template<typename Type>
class MonoBehaviour:public Behaviour<Type>
{
public:
	//�R���X�g���N�^
	MonoBehaviour(EntityID OwnerID);
	//�f�X�g���N�^
	virtual ~MonoBehaviour() = default;

	//����������
	virtual void Start() {};
	//�X�V����
	virtual void Update(){};
	//���X�V����
	virtual void FixedUpdate(){};

	//�폜�����s�֐�
	virtual void OnDestroy()	override {};

};

template<typename Type>
inline MonoBehaviour<Type>::MonoBehaviour(EntityID OwnerID)
:
	Behaviour<Type>(OwnerID)
{
	IComponent::SendComponentMessage = [this](std::string message)
	{
		if (message == "Start") { this->Start(); return; }
		if (message == "Update") { this->Update(); return; }
		if (message == "FixedUpdate") { this->FixedUpdate(); return; }
	};
}