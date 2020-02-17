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


public:
	//�R���X�g���N�^
	Object();
	//�f�X�g���N�^
	virtual ~Object();

	//�ŗLID�̎擾
	InstanceID GetInstanceID();

	//�폜�錾 DestroyIndex�֒ǉ�
	virtual void Destroy();


protected:
	//�����j���֐�
	virtual void Release() = 0;

	//ObjectManager�o�^�����s�֐�
	virtual void Register(std::shared_ptr<Object> instance) {};


private:
	//�V���A���C�Y
	template<class Archive>
	void save(Archive& archive) const 
	{

	}

	//�f�V���A���C�Y
	template<class Archive>
	void load(Archive& archive)
	{

	}


};

CEREAL_REGISTER_TYPE(Object)