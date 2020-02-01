#include<assert.h>
#include<random>
#include"Common.h"

#include"Object.h"
#include"ObjectManager.h"

std::random_device rand;

//*********************************************************************************************************************
//
//	Object
//
//*********************************************************************************************************************

//Object
//	�R���X�g���N�^
//
Object::Object()
	:
	_InstanceID(rand())
{
	
}

//~Object
//	�f�X�g���N�^
//
Object::~Object()
{
	if(_self) _self.reset();
}

//OnDestroy
//	�폜�����s�֐�
//
void Object::OnDestroy()
{

}

//GetInstanceID
//	InstanceID�̎擾
//
InstanceID Object::GetInstanceID()
{
	return _InstanceID;
}

std::shared_ptr<Object> Object::GetSelf()
{
	if (!_self)
		_self = ObjectManager::GetInstance()->GetObjectInstance(_InstanceID);
	return _self;
}
