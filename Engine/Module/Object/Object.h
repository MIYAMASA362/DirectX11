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
//		�Q�[�����̃I�u�W�F�N�g�̊��
//
//*********************************************************************************************************************
class Object
{
	friend class ObjectManager;
	friend cereal::access;
private:
	//�ŗLID
	InstanceID _InstanceID;
	//ObjectManager�ŊǗ�����Ă���Instacne�ւ̃A�N�Z�X
	std::weak_ptr<Object> _self;

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
	Object(InstanceID id,std::shared_ptr<Object> sptr);
	//�f�X�g���N�^
	virtual ~Object();

	//Object�̍폜����
	static void Destroy(Object* obj);

	//�ŗLID�̎擾
	InstanceID GetInstanceID();

	//�폜�錾
	virtual void Destroy();

	//_self�̎擾
	std::weak_ptr<Object> GetSelf();

protected:
	//�폜�����s�֐�
	virtual void OnDestroy();


};

CEREAL_REGISTER_TYPE(Object)