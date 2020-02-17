#include<assert.h>
#include<random>
#include"Common.h"

#include"Object.h"
#include"ObjectManager.h"

//*********************************************************************************************************************
//
//	Object
//
//*********************************************************************************************************************

static std::random_device random;

//Object
//	�R���X�g���N�^
//
Object::Object()
	:
	_InstanceID(random())
{

}

//~Object
//	�f�X�g���N�^
//
Object::~Object()
{

}

//GetInstanceID
//	InstanceID�̎擾
//
InstanceID Object::GetInstanceID()
{
	return _InstanceID;
}

//Destroy
//	�폜�錾
//
void Object::Destroy()
{
	ObjectManager::GetInstance()->DestroyObject(this);
}