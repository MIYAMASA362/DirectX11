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

protected:
	//�폜�����s�֐�
	virtual void OnDestroy()	override {};

	virtual void OnDebugImGui() override {};

	virtual void SendComponentMessage(std::string message);
};

template<typename Type>
inline MonoBehaviour<Type>::MonoBehaviour(EntityID OwnerID)
:
	Behaviour<Type>(OwnerID)
{

}

template<typename Type>
inline void MonoBehaviour<Type>::SendComponentMessage(std::string message)
{
	if (message == "Start") return Start();
	if (message == "Update") return Update();
	if (message == "FixedUpdate") return FixedUpdate();
}
