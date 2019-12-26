#pragma once

//Behavuour
//	動作をするかの切り替え
//
template<typename Type>
class Behaviour:public Component<Type>
{	
protected:
	//Componentの有効・無効
	bool _IsEnable = true;


public:
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


};