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
//	コンストラクタ
//
Object::Object()
	:
	_InstanceID(NULL)
{
	
}

//Object
//	ObjectManagerによって登録された際に利用
//
Object::Object(InstanceID id, std::shared_ptr<Object> sptr)
	:
	_InstanceID(id),
	_self(sptr)
{

}

//~Object
//	デストラクタ
//
Object::~Object()
{

}

//OnDestroy
//	削除時実行関数
//
void Object::OnDestroy()
{

}

//Destory
//	削除関数
//
void Object::Destroy(Object* obj)
{
	obj->Destroy();
}

//GetInstanceID
//	InstanceIDの取得
//
InstanceID Object::GetInstanceID()
{
	if (_self.expired()) assert(0);
	return _InstanceID;
}

//Destory
//	削除宣言
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
