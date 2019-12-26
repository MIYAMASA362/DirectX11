#pragma once

//MonoBehaviour
//	逐次処理を追加
//
template<typename Type>
class MonoBehaviour:public Behaviour<Type>
{
public:
	//コンストラクタ
	MonoBehaviour(EntityID OwnerID);
	//デストラクタ
	virtual ~MonoBehaviour() = default;

	//初期化処理
	virtual void Start() {};
	//更新処理
	virtual void Update(){};
	//一定更新処理
	virtual void FixedUpdate(){};

	//削除時実行関数
	virtual void OnDestroy()	override {};

};

template<typename Type>
inline MonoBehaviour<Type>::MonoBehaviour(EntityID OwnerID)
:
	Behaviour<Type>(OwnerID)
{
	IComponent::SendComponentMessage = [this](std::string message)
	{
		if (message == "Start") { this->Start(); return; }
		if (message == "Update") { this->Update(); return; }
		if (message == "FixedUpdate") { this->FixedUpdate(); return; }
	};
}