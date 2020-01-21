#pragma once


class Collider;

//Rigidbody
//	物理挙動を設定する
//
class Rigidbody final :public Component<Rigidbody>
{
private:
	//重力の有効・無効
	bool _UseGravity;
	//加速度
	Vector3 _Velocity;
	//質量
	float _Mass;
	//前位置
	Vector3 _OldPosition;
	//衝突判定
	std::list<std::weak_ptr<Collider>> _Colliders;


public:
	//物理判定を適応
	static void ApplyRigidbody();
	//謎...
	static void CollisionRigidbody();

	//コンスタント
	Rigidbody(EntityID OwnerID);
	//デストラクタ
	virtual ~Rigidbody();

	//useGravityの切り替え
	void IsUseGravity(bool enable) { _UseGravity = enable; }
	
	//重力取得
	Vector3 GetVelocity() { return _Velocity; };
	//加速度設定
	void SetVelocity(Vector3 velocity) { _Velocity = velocity; };
	
	//質量設定
	void SetMass(float mass) { _Mass = mass; };
	//Colliderへ追加
	void RegisterCollider(std::weak_ptr<Collider> collider) { _Colliders.push_back(collider); };

	//力を追加
	void AddForce(Vector3 force) { _Velocity = _Mass * force; }
	//前位置取得
	Vector3 GetOldPosition() { return this->_OldPosition; }
	//一定時間更新
	void FixedUpdate();


protected:
	//削除時実行関数
	void OnDestroy() override;

	void OnDebugImGui() override;

	void SendComponentMessage(std::string message) override;
};
