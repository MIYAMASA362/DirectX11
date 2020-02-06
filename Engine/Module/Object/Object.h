#pragma once

//Object�ŗLID
using InstanceID = unsigned int;

namespace cereal
{
	class access;
}

//*********************************************************************************************************************
//
//	Object
//		�ŗL�l�����������ʃI�u�W�F�N�g
//
//*********************************************************************************************************************
class Object
{
	friend class ObjectManager;
	friend cereal::access;
private:
	//�ŗLID
	InstanceID _InstanceID;

	//�V���A����
	template<class Archive>
	void save(Archive& archive) const 
	{
		archive(
			CEREAL_NVP(_InstanceID)
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(_InstanceID)
		);
	}

public:
	//�R���X�g���N�^
	Object();
	//�f�X�g���N�^
	virtual ~Object();

	//�ŗLID�̎擾
	InstanceID GetInstanceID();

	virtual void Destroy() = 0;
};

CEREAL_REGISTER_TYPE(Object)