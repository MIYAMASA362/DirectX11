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
//	コンストラクタ
//
Object::Object()
	:
	_InstanceID(random())
{

}

//~Object
//	デストラクタ
//
Object::~Object()
{

}

//GetInstanceID
//	InstanceIDの取得
//
InstanceID Object::GetInstanceID()
{
	return _InstanceID;
}

//Destroy
//	削除宣言
//
void Object::Destroy()
{
	ObjectManager::GetInstance()->DestroyObject(this);
}