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

protected:
	//削除時実行関数
	virtual void OnDestroy()	override {};

	virtual void OnDebugImGui() override {};

	virtual void SendComponentMessage(std::string message);
};

template<typename Type>
inline MonoBehaviour<Type>::MonoBehaviour(EntityID OwnerID)
:
	Behaviour<Type>(OwnerID)
{

}

template<typename Type>
inline void MonoBehaviour<Type>::SendComponentMessage(std::string message)
{
	if (message == "Start") return Start();
	if (message == "Update") return Update();
	if (message == "FixedUpdate") return FixedUpdate();
}
