#pragma once
namespace DirectX
{
	class GameObject;
	class Hierarchy;

	//変換Component
	class Transform :public Component<Transform>
	{
	private:
		Hierarchy*  _hierarchy;		//親子
	protected:
		Vector3		m_Position;		//位置
		Quaternion	m_Rotation;		//回転
		Vector3		m_Scale;		//サイズ
	public:
		Transform(EntityID OwnerID);
		~Transform() = default;

		Vector3		position();								//ワールド位置
		Quaternion	rotation();								//ワールド回転
		Vector3		scale();								//ワールド大きさ

		Vector3		localPosition();						//ローカル位置
		Quaternion	localRotation();						//ローカル回転
		Vector3		localScale();							//ローカル大きさ

		void position(Vector3 position);					//ワールド位置
		void rotation(Quaternion rotation);					//ワールド回転

		void localPosition(Vector3 position);				//ローカル位置
		void localRotation(Quaternion rotation);			//ローカル回転
		void localScale(Vector3 scale);						//ローカル大きさ
	protected:
		void detachParent();								//親を離す
		void detachChild(std::weak_ptr<Transform> child);	//親がターゲットの子を見つけると削除する
		void childTransformUpdate();						//子の行列に変更を加える
	public:
		void SetParent(std::weak_ptr<Transform> pParent);	//親子を設定
		void SetParent(std::weak_ptr<GameObject> parent);
		void DetachParent();								//親を離す
		void DetachChildren();								//子を離す
		std::weak_ptr<Transform> GetParent();				//親取得
		std::list<std::weak_ptr<Transform>> GetChildren();	//子取得

		void SendComponentMessageChildren(std::string message);	//自身と子のComponentsにmessageを送る
		std::weak_ptr<IComponent> GetComponentInParent(ComponentTypeID componentTypeID);
		std::weak_ptr<IComponent> GetComponentInChildren(ComponentTypeID componentTypeID);
		Components GetComponentsInChildren(ComponentTypeID componentTypeID);
	protected:
		Vector3 TransformDirection(Vector3 direction);	//回転行列を使ってDirectionを変換
	public:
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
	};
}