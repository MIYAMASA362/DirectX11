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
//		固有値を持った識別オブジェクト
//
//*********************************************************************************************************************
class Object
{
	friend class ObjectManager;
	friend cereal::access;
private:
	//固有ID
	InstanceID _InstanceID;


public:
	//コンストラクタ
	Object();
	//デストラクタ
	virtual ~Object();

	//固有IDの取得
	InstanceID GetInstanceID();

	//削除宣言 DestroyIndexへ追加
	virtual void Destroy();


protected:
	//即時破棄関数
	virtual void Release() = 0;

	//ObjectManager登録時実行関数
	virtual void Register(std::shared_ptr<Object> instance) {};


private:
	//シリアライズ
	template<class Archive>
	void save(Archive& archive) const 
	{

	}

	//デシリアライズ
	template<class Archive>
	void load(Archive& archive)
	{

	}


};

CEREAL_REGISTER_TYPE(Object)