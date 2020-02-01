#pragma once

class GameObject;
class Hierarchy;

namespace cereal
{
	class access;
}

//*********************************************************************************************************************
//
//	Transform
//
//*********************************************************************************************************************
class Transform final : public Component<Transform>
{
	friend cereal::access;
private:
	//親子
	Hierarchy*  _hierarchy;

	//位置
	Vector3	_Position;
	//回転
	Quaternion _Rotation;
	//スケール
	Vector3 _Scale;


	//コンストラクタ
	Transform();

public:
	//コンストラクタ
	Transform(EntityID OwnerID);
	//デストラクタ
	~Transform();

	//位置 取得
	Vector3	position();			//ワールド
	Vector3	localPosition();	//ローカル

	//回転 取得
	Quaternion rotation();		//ワールド
	Quaternion localRotation(); //ローカル

	//スケール 取得
	Vector3	scale();			//ワールド
	Vector3	localScale();		//ローカル

	//位置 設定
	void position(Vector3 position);					//ワールド位置
	void localPosition(Vector3 position);				//ローカル位置

	//回転 設定
	void rotation(Quaternion rotation);					//ワールド回転
	void localRotation(Quaternion rotation);			//ローカル回転

	//スケール設定
	void localScale(Vector3 scale);						//ローカル大きさ

	//親子関係
	void SetParent(std::weak_ptr<Transform> pParent);	//親子を設定
	void SetParent(std::weak_ptr<GameObject> parent);
	void DetachParent();								//親を離す
	void DetachChildren();								//子を離す
	std::weak_ptr<Transform> GetParent();				//親取得
	std::list<std::weak_ptr<Transform>> GetChildren();	//子取得

	void SendComponentMessageChildren(std::string message);	//自身と子のComponentsにmessageを送る
	std::weak_ptr<IComponent> GetComponentInParent(ComponentTypeID componentTypeID);
	std::weak_ptr<IComponent> GetComponentInChildren(ComponentTypeID componentTypeID);
	ComponentList GetComponentsInChildren(ComponentTypeID componentTypeID);

	Vector3 right();
	Vector3 left();
	Vector3 up();
	Vector3 down();
	Vector3 forward();
	Vector3 back();

	XMMATRIX MatrixQuaternion();		//回転行列
	XMMATRIX MatrixTranslation();		//移動行列
	XMMATRIX MatrixScaling();			//スケール行列
	XMMATRIX WorldMatrix();				//ワールド行列

	void LookAt(std::weak_ptr<Transform> target);	//その方向を見る
	void OnDestroy() override;

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Component<Transform>>(this),
			CEREAL_NVP(_Position),
			CEREAL_NVP(_Rotation),
			CEREAL_NVP(_Scale)
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Component<Transform>>(this),
			CEREAL_NVP(_Position),
			CEREAL_NVP(_Rotation),
			CEREAL_NVP(_Scale)
		);
	}

protected:
	virtual IComponent* Internal_CreateInstance(IEntity* owner) override;

	void detachParent();								//親を離す
	void detachChild(std::weak_ptr<Transform> child);	//親がターゲットの子を見つけると削除する
	void childTransformUpdate();						//子の行列に変更を加える

	Vector3 TransformDirection(Vector3 direction);	//回転行列を使ってDirectionを変換

	void OnDebugImGui() override;
};

CEREAL_REGISTER_TYPE(Component<Transform>)
CEREAL_REGISTER_TYPE(Transform)