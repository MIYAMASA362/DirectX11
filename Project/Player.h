#pragma once

class Player:public Component<Player>
{
public:
	const float speed = 0.5f;
	FieldCollider* _fieldCollider;

	virtual void SendComponentMessage(std::string message) override
	{
		if (message == "Update") Update();
		if (message == "FixedUpdate") FixedUpdate();
	}

	void Update()
	{
		
	}

	void FixedUpdate()
	{
		float horizontal = Input::Mouse::GetAccelerationX();
		this->gameObject()->transform().lock()->localRotation(
			this->gameObject()->transform().lock()->localRotation() * Quaternion::AngleAxisToEuler(horizontal,this->gameObject()->transform().lock()->up())
		);

		if (Input::GetKeyPress('W'))
			this->gameObject()->transform().lock()->position(this->gameObject()->transform().lock()->position() + this->gameObject()->transform().lock()->forward() * speed);
		if (Input::GetKeyPress('S'))
			this->gameObject()->transform().lock()->position(this->gameObject()->transform().lock()->position() + this->gameObject()->transform().lock()->back() *speed);
		if (Input::GetKeyPress('A'))
			this->gameObject()->transform().lock()->position(this->gameObject()->transform().lock()->position() + this->gameObject()->transform().lock()->left() *speed);
		if (Input::GetKeyPress('D'))
			this->gameObject()->transform().lock()->position(this->gameObject()->transform().lock()->position() + this->gameObject()->transform().lock()->right()*speed);
		
		/*if (_fieldCollider->IsOnGround(this->gameObject()->transform().lock()->position()))
		{
			Vector3 pos = this->gameObject()->transform().lock()->position();
			pos.y = _fieldCollider->GetHeight(pos) + 4.0f;
			this->gameObject()->transform().lock()->position(pos);
			this->gameObject()->GetComponent<Rigidbody>().lock()->SetVelocity(Vector3::zero());
		}
*/
		RayCastHit info;
		if (Physics::RayCast(this->gameObject()->transform().lock()->position() + Vector3::up()*2.0f, Vector3::down(), info, 10.0f))
		{
			Vector3 pos = this->gameObject()->transform().lock()->position();
			pos.y = info.hitPosition.y + this->gameObject()->transform().lock()->scale().y;
			this->gameObject()->transform().lock()->position(pos);
		}
	}
};

class PlayerCamera:public Component<PlayerCamera>
{
public:
	GameObject* target;
	GameObject* HorizonPivot;
	GameObject* VerticalPivot;
	GameObject* player;
	const float speed = 0.001f;

	virtual void SendComponentMessage(std::string message) override
	{
		if (message == "Update") Update();
	}


	void Update()
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

class PlayerMove:public Component<PlayerMove>
{
private:
	const float walk = 0.8f;	//歩く
	const float run = 1.5f;		//走る
	const float speed = 0.5f;	//基準速度

	bool IsRun = false;	//走っているか
public:
	Camera* camera;

	virtual void SendComponentMessage(std::string message) override
	{
		if (message == "Update") Update();
	}

	void Update()
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
