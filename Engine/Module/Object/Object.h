#pragma once

//Object固有ID
using InstanceID = unsigned int;

//Object
//	ゲーム内のオブジェクトの基底
//
class Object
{
	friend class ObjectManager;
private:
	//固有ID
	const InstanceID _InstanceID;

protected:
	//ObjectManagerで管理されているInstacneへのアクセス
	std::weak_ptr<Object> _self;

public:
	//コンストラクタ
	Object();
	//デストラクタ
	virtual ~Object();

	//Objectの削除処理
	static void Destroy(Object* obj);

	//固有IDの取得
	InstanceID GetInstanceID() { return _InstanceID; }

	//削除宣言
	virtual void Destroy();


protected:
	//削除時実行関数
	virtual void OnDestroy();


};