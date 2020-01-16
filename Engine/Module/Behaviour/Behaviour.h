#pragma once

//*********************************************************************************************************************
//
//	Behaviour Component
//
//*********************************************************************************************************************
template<typename Type>
class Behaviour:public Component<Type>
{	
protected:
	//Componentの有効・無効
	bool _IsEnable = true;


public:
	Behaviour() :Behaviour(0) {};
	//コンストラクタ
	Behaviour(EntityID OwnerID) :Component(OwnerID) {};
	//デストラクタ
	virtual ~Behaviour() = default;

	//IsEnable切り替え
	void SetEnable(bool enable) { return _IsEnable = enable; }
	//IsEnable取得
	bool GetEnable() { return _IsEnable; }

	//削除時実行関数
	virtual void OnDestroy()	override {};

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<Component<Type>>(this));
	}
};
