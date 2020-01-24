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
	Behaviour(){};
	//�R���X�g���N�^
	Behaviour(EntityID OwnerID) :Component(OwnerID) {};
	//�f�X�g���N�^
	virtual ~Behaviour() = default;

	//IsEnable�؂�ւ�
	void SetEnable(bool enable) { return _IsEnable = enable; }
	//IsEnable�擾
	bool GetEnable() { return _IsEnable; }

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::base_class<Component<Type>>(this));
	}
	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::base_class<Component<Type>>(this));
	}

protected:
	//�폜�����s�֐�
	virtual void OnDestroy()	override { }

	virtual void OnDebugImGui() override { Component<Type>::OnDebugImGui(); }

	virtual void SendComponentMessage(std::string message) {}
};
