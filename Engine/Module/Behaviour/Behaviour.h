#pragma once

//*********************************************************************************************************************
//
//	Behaviour Component
//
//*********************************************************************************************************************
template<typename Type>
class Behaviour:public Component<Type>
{	
protected:
	//Component�̗L���E����
	bool _IsEnable = true;


public:
	Behaviour() :Behaviour(0) {};
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

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<Component<Type>>(this));
	}
};
