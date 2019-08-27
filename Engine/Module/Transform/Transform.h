#pragma once
namespace DirectX
{
	class GameObject;

	//変換Component
	class Transform :public Component
	{
	//--- Attribute -------------------------------------------------
	protected:
		std::weak_ptr<Transform> pParent;					//親
		std::list<std::weak_ptr<Transform>> pChildren;		//子
	protected:
		Vector3		m_Position;		//位置
		Quaternion	m_Rotation;		//回転
		Vector3		m_Scale;		//サイズ

		XMMATRIX	m_WorldMatrix;	//ワールド行列

	//--- Constrcutor/Destrcutor ------------------------------------
	public:
		Transform(Vector3 position, Quaternion rotation, Vector3 scale);
		Transform(Vector3 position, Vector3 rotation, Vector3 scale);
		Transform();
		~Transform() = default;

	//--- Getter ----------------------------------------------------
	public:
		Vector3		position();								//ワールド位置
		Quaternion	rotation();								//ワールド回転
		Vector3		scale();								//ワールド大きさ

		Vector3		localPosition();						//ローカル位置
		Quaternion	localRotation();						//ローカル回転
		Vector3		localScale();							//ローカル大きさ

	//--- Setter ----------------------------------------------------
	public:
		void position(Vector3 position);					//ワールド位置
		void rotation(Quaternion rotation);					//ワールド回転

		void localPosition(Vector3 position);				//ローカル位置
		void localRotation(Quaternion rotation);			//ローカル回転
		void localScale(Vector3 scale);						//ローカル大きさ

	//--- 親子関係 --------------------------------------------------
	protected:
		void detachParent();								//親を離す
		void detachChildSearch(Transform* target);			//親がターゲットの子を見つけると削除する
		void childTransformUpdate();						//子の行列に変更を加える
	public:
		void SetParent(std::weak_ptr<Transform> parent);	//親子を設定
		void SetParent(GameObject* parent);
		void DetachParent();								//親を離す
		void DetachChildren();								//子を離す
		std::weak_ptr<Transform> GetParent();				//親取得
		std::list<std::weak_ptr<Transform>> GetChildren();	//子取得
		void SendComponentMessageChildren(Component::Message message);

	//--- Direction -------------------------------------------------
	protected:
		Vector3 TransformDirection(Vector3 direction);	//回転行列を使ってDirectionを変換
	public:
		Vector3 right();			//右
		Vector3 left();				//左
		Vector3 up();				//上
		Vector3 down();				//下
		Vector3 forward();			//前
		Vector3 back();				//後

	//--- Matrix ----------------------------------------------------
	public:
		XMMATRIX MatrixQuaternion();		//回転行列
		XMMATRIX MatrixTranslation();		//移動行列
		XMMATRIX MatrixScaling();			//スケール行列
		XMMATRIX WorldMatrix();				//ワールド行列
	public:
		void LookAt(std::weak_ptr<Transform> target);	//その方向を見る
		virtual const std::type_info& GetType() override { return typeid(*this); };
	};
}