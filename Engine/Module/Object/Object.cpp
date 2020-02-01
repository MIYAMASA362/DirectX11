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
//	コンストラクタ
//
Object::Object()
	:
	_InstanceID(rand())
{
	
}

//~Object
//	デストラクタ
//
Object::~Object()
{
	if(_self) _self.reset();
}

//OnDestroy
//	削除時実行関数
//
void Object::OnDestroy()
{

}

//GetInstanceID
//	InstanceIDの取得
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
