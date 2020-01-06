#pragma once

//Object�ŗLID
using InstanceID = unsigned int;

//Object
//	�Q�[�����̃I�u�W�F�N�g�̊��
//
class Object
{
	friend class ObjectManager;
private:
	//�ŗLID
	const InstanceID _InstanceID;

protected:
	//ObjectManager�ŊǗ�����Ă���Instacne�ւ̃A�N�Z�X
	std::weak_ptr<Object> _self;

public:
	//�R���X�g���N�^
	Object();
	//�f�X�g���N�^
	virtual ~Object();

	//Object�̍폜����
	static void Destroy(Object* obj);

	//�ŗLID�̎擾
	InstanceID GetInstanceID() { return _InstanceID; }

	//�폜�錾
	virtual void Destroy();


protected:
	//�폜�����s�֐�
	virtual void OnDestroy();


};