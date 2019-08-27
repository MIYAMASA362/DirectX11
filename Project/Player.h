#pragma once

class Player:public Behaviour
{
public:
	Player() :Behaviour("Player") {};

	void Update()
	{
		
	}

	void OnTriggerEnter(std::weak_ptr<Collider> other) override
	{
		if(other.lock()->gameObject.lock()->CompareTag(TagName::Enemy))
		{
			if (Input::Mouse::IsLeftTrigger())
				other.lock()->gameObject.lock()->GetComponent<Enemy>().lock()->Damage();
		}
	}
};

class PlayerCamera:public Behaviour
{
public:
	GameObject* target;
	GameObject* HorizonPivot;
	GameObject* VerticalPivot;
	GameObject* player;
	const float speed = 0.001f;
public:
	PlayerCamera() :Behaviour("PlayerCamera") {};

	void Update() override
	{
		float horizontal = Input::Mouse::GetAccelerationX() * speed;
		float vertical = Input::Mouse::GetAccelerationY() * speed;

		HorizonPivot->transform->localRotation(HorizonPivot->transform->localRotation()*Quaternion::AngleAxisToRadian(horizontal,Vector3::up()));
		VerticalPivot->transform->localRotation(VerticalPivot->transform->localRotation()*Quaternion::AngleAxisToRadian(vertical,Vector3::right()));

		//カメラリセット
		if (Input::GetKeyTrigger('Q')) {
			HorizonPivot->transform->localRotation(player->transform->localRotation());
			VerticalPivot->transform->localRotation(Quaternion::Identity());
		}

		//ターゲットカメラ切替
		if (Input::GetKeyTrigger('F')) {
			HorizonPivot->transform->LookAt(target->transform);

			//this->transform.lock()->LookAt(target->transform);
		}
	}
};

class PlayerMove:public Behaviour
{
private:
	const float walk = 0.8f;	//歩く
	const float run = 1.5f;		//走る
	const float speed = 0.5f;	//基準速度

	bool IsRun = false;	//走っているか
public:
	Camera* camera;
public:
	PlayerMove() :Behaviour("PlayerMove") {};
	void Update() override
	{
		Vector3 vec = Vector3::zero();
		float horizontal = 0.0f;
		bool IsInput = false;

		if (Input::GetKeyPress('W')) {
			vec += camera->transform.lock()->forward();
			IsInput = true;
		}
		else if (Input::GetKeyPress('S')) {
			vec += camera->transform.lock()->back();
			IsInput = true;
		}
		if (Input::GetKeyPress('A')) {
			vec += camera->transform.lock()->left();
			IsInput = true;
		}
		else if (Input::GetKeyPress('D')) {
			vec += camera->transform.lock()->right();
			IsInput = true;
		}

		//走る・歩く切替
		if (Input::GetKeyTrigger('U'))
			IsRun = !IsRun;

		if (IsInput)
			this->transform.lock()->localRotation(Quaternion::QuaternionLookRotation(Vector3(vec.x,0.0f,vec.z), Vector3::up()));
		vec = vec.normalize();
		vec *= IsRun ? run * speed : walk * speed;

		auto rigidbody = this->gameObject.lock()->rigidbody;
		if(rigidbody)
			rigidbody->AddForce(vec);
	}
};

class PivotBehaviour:public Behaviour
{
public:
	GameObject* target;
	PivotBehaviour() :Behaviour("PivotBehaviour") {};
public:
	void Update(){
		this->transform.lock()->position(target->transform->position());
	}
};