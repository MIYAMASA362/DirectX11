#include<assert.h>
#include"Common.h"

#include"Object.h"
#include"ObjectManager.h"

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
	_InstanceID(NULL)
{
	
}

//Object
//	ObjectManager�ɂ���ēo�^���ꂽ�ۂɗ��p
//
Object::Object(InstanceID id, std::shared_ptr<Object> sptr)
	:
	_InstanceID(id),
	_self(sptr)
{

}

//~Object
//	�f�X�g���N�^
//
Object::~Object()
{

}

//OnDestroy
//	�폜�����s�֐�
//
void Object::OnDestroy()
{

}

//Destory
//	�폜�֐�
//
void Object::Destroy(Object* obj)
{
	obj->Destroy();
}

//GetInstanceID
//	InstanceID�̎擾
//
InstanceID Object::GetInstanceID()
{
	if (_self.expired()) assert(0);
	return _InstanceID;
}

//Destory
//	�폜�錾
//
void Object::Destroy()
{
	ObjectManager::GetInstance()->DestroyObject(this);
}

std::weak_ptr<Object> Object::GetSelf()
{
	if (_self.expired()) assert(0);
	return _self;
}
