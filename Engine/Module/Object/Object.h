#pragma once

//Object固有ID
using InstanceID = unsigned int;

namespace cereal
{
	class access;
}

//*********************************************************************************************************************
//
//	Object
//		ゲーム内のオブジェクトの基底
//
//*********************************************************************************************************************
class Object
{
	friend class ObjectManager;
	friend cereal::access;
private:
	//固有ID
	InstanceID _InstanceID;
	//ObjectManagerで管理されているInstacneへのアクセス
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
	//コンストラクタ
	Object();
	Object(InstanceID id,std::shared_ptr<Object> sptr);
	//デストラクタ
	virtual ~Object();

	//Objectの削除処理
	static void Destroy(Object* obj);

	//固有IDの取得
	InstanceID GetInstanceID();

	//削除宣言
	virtual void Destroy();

	//_selfの取得
	std::weak_ptr<Object> GetSelf();

protected:
	//削除時実行関数
	virtual void OnDestroy();


};

CEREAL_REGISTER_TYPE(Object)