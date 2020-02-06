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

	//シリアル化
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
	//デストラクタ
	virtual ~Object();

	//固有IDの取得
	InstanceID GetInstanceID();

	virtual void Destroy() = 0;
};

CEREAL_REGISTER_TYPE(Object)