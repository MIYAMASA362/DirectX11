#pragma once

//Behavuour
//	��������邩�̐؂�ւ�
//
template<typename Type>
class Behaviour:public Component<Type>
{	
protected:
	//Component�̗L���E����
	bool _IsEnable = true;


public:
	//�R���X�g���N�^
	Behaviour(EntityID OwnerID) :Component(OwnerID) {};
	//�f�X�g���N�^
	virtual ~Behaviour() = default;

	//IsEnable�؂�ւ�
	void SetEnable(bool enable) { return _IsEnable = enable; }
	//IsEnable�擾
	bool GetEnable() { return _IsEnable; }

	//�폜�����s�֐�
	virtual void OnDestroy()	override {};


};