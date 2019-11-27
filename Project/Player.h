#pragma once

class Player:public MonoBehaviour<Player>
{
public:
	const float speed = 0.5f;
	FieldCollider* _fieldCollider;
public:
	Player(EntityID OwnerID) :MonoBehaviour(OwnerID) {};

	void Update()
	{
		
	}

	void FixedUpdate()
	{
		float horizontal = Input::Mouse::GetAccelerationX();
		this->transform()->localRotation(
			this->transform()->localRotation() * Quaternion::AngleAxisToEuler(horizontal,this->transform()->up())
		);

		if (Input::GetKeyPress('W'))
			this->transform()->position(this->transform()->position() + this->transform()->forward() * speed);
		if (Input::GetKeyPress('S'))
			this->transform()->position(this->transform()->position() + this->transform()->back() *speed);
		if (Input::GetKeyPress('A'))
			this->transform()->position(this->transform()->position() + this->transform()->left() *speed);
		if (Input::GetKeyPress('D'))
			this->transform()->position(this->transform()->position() + this->transform()->right()*speed);
		
		/*if (_fieldCollider->IsOnGround(this->transform()->position()))
		{
			Vector3 pos = this->transform()->position();
			pos.y = _fieldCollider->GetHeight(pos) + 4.0f;
			this->transform()->position(pos);
			this->gameObject()->GetComponent<Rigidbody>().lock()->SetVelocity(Vector3::zero());
		}
*/
		RayCastHit info;
		if (Physics::RayCast(this->transform()->position() + Vector3::up()*2.0f, Vector3::down(), info, 10.0f))
		{
			Vector3 pos = this->transform()->position();
			pos.y = info.hitPosition.y + this->transform()->scale().y;
			this->transform()->position(pos);
		}
	}
};

class PlayerCamera:public MonoBehaviour<PlayerCamera>
{
public:
	GameObject* target;
	GameObject* HorizonPivot;
	GameObject* VerticalPivot;
	GameObject* player;
	const float speed = 0.001f;
public:
	PlayerCamera(EntityID OwnerID) :MonoBehaviour(OwnerID) {};

	void Update() override
	{
		//float horizontal = Input::Mouse::GetAccelerationX() * speed;
		//float vertical = Input::Mouse::GetAccelerationY() * speed;

		//HorizonPivot->transform->localRotation(HorizonPivot->transform->localRotation()*Quaternion::AngleAxisToRadian(horizontal,Vector3::up()));
		//VerticalPivot->transform->localRotation(VerticalPivot->transform->localRotation()*Quaternion::AngleAxisToRadian(vertical,Vector3::right()));

		////カメラリセット
		//if (Input::GetKeyTrigger('Q')) {
		//	HorizonPivot->transform->localRotation(player->transform->localRotation());
		//	VerticalPivot->transform->localRotation(Quaternion::Identity());
		//}

		////ターゲットカメラ切替
		//if (Input::GetKeyTrigger('F')) {
		//	HorizonPivot->transform->LookAt(target->transform);

		//	//this->transform.lock()->LookAt(target->transform);
		//}
	}
};

class PlayerMove:public MonoBehaviour<PlayerMove>
{
private:
	const float walk = 0.8f;	//歩く
	const float run = 1.5f;		//走る
	const float speed = 0.5f;	//基準速度

	bool IsRun = false;	//走っているか
public:
	Camera* camera;
public:
	PlayerMove(EntityID OwnerID) :MonoBehaviour(OwnerID) {};
	void Update() override
	{
		//Vector3 vec = Vector3::zero();
		//float horizontal = 0.0f;
		//bool IsInput = false;

		//if (Input::GetKeyPress('W')) {
		//	vec += camera->transform.lock()->forward();
		//	IsInput = true;
		//}
		//else if (Input::GetKeyPress('S')) {
		//	vec += camera->transform.lock()->back();
		//	IsInput = true;
		//}
		//if (Input::GetKeyPress('A')) {
		//	vec += camera->transform.lock()->left();
		//	IsInput = true;
		//}
		//else if (Input::GetKeyPress('D')) {
		//	vec += camera->transform.lock()->right();
		//	IsInput = true;
		//}

		////走る・歩く切替
		//if (Input::GetKeyTrigger('U'))
		//	IsRun = !IsRun;

		//if (IsInput)
		//	this->transform.lock()->localRotation(Quaternion::QuaternionLookRotation(Vector3(vec.x,0.0f,vec.z), Vector3::up()));
		//vec = vec.normalize();
		//vec *= IsRun ? run * speed : walk * speed;

		//auto rigidbody = this->gameObject.lock()->rigidbody;
		//if(rigidbody)
		//	rigidbody->AddForce(vec);
	}
};

class PivotBehaviour:public MonoBehaviour<PivotBehaviour>
{
public:
	GameObject* target;
	PivotBehaviour(EntityID OwnerID) :MonoBehaviour(OwnerID) {};
public:
	void Update(){
		/*this->transform.lock()->position(target->transform->position());*/
	}
};