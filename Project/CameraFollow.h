#pragma once

class LookAt:public Component<LookAt>
{
public:
	GameObject* parent;
public:
	virtual void SendComponentMessage(std::string message) override
	{
		if (message == "Update") Update();
	}

	void Update()
	{
		//this->transform.lock()->LookAt(parent->transform);
	}
	/*void OnTriggerEnter(std::weak_ptr<Collider> other) override{
		if (other.lock()->gameObject.lock()->CompareTag(TagName::ChildMiku))
			this->gameObject.lock()->Destroy();
	}*/
};

class WASDMove:public Component<WASDMove>
{
public:
	const float speed = 0.5f;
	FieldCollider* _fieldCollider;
	bool IsGround = false;
public:
	virtual void SendComponentMessage(std::string message) override
	{
		if (message == "Update") Update();
		if (message == "FixedUpdate") FixedUpdate();
	}

	void Update()
	{
		if (Input::GetKeyPress('W'))
			this->gameObject()->transform().lock()->position(this->gameObject()->transform().lock()->position() + this->gameObject()->transform().lock()->forward() * speed);
		if (Input::GetKeyPress('S'))
			this->gameObject()->transform().lock()->position(this->gameObject()->transform().lock()->position() + this->gameObject()->transform().lock()->back() *speed);
		if (Input::GetKeyPress('A'))
			this->gameObject()->transform().lock()->position(this->gameObject()->transform().lock()->position() + this->gameObject()->transform().lock()->left() *speed);
		if (Input::GetKeyPress('D'))
			this->gameObject()->transform().lock()->position(this->gameObject()->transform().lock()->position() + this->gameObject()->transform().lock()->right()*speed);
		if (Input::GetKeyPress('Q'))
			this->gameObject()->transform().lock()->position(this->gameObject()->transform().lock()->position() + this->gameObject()->transform().lock()->down()*speed);
		if (Input::GetKeyPress('E'))
			this->gameObject()->transform().lock()->position(this->gameObject()->transform().lock()->position() + this->gameObject()->transform().lock()->up()*speed);

		//this->transform()->position(Camera::ScreenToWorldPosition(Vector3::zero()));

		
		/*auto rigidbody = this->gameObject()->GetComponent<Rigidbody>();
		if (rigidbody != nullptr)
			if (Input::GetKeyTrigger(VK_SPACE))
				rigidbody->AddForce(Vector3::up() * 5.0f);*/		
	}

	void FixedUpdate()
	{
		IsGround = _fieldCollider->IsOnGround(this->gameObject()->transform().lock()->position());
		if (IsGround)
		{
			Vector3 pos = this->gameObject()->transform().lock()->position();
			pos.y = _fieldCollider->GetHeight(pos) + 4.0f;
			this->gameObject()->transform().lock()->position(pos);
			this->gameObject()->GetComponent<Rigidbody>()->SetVelocity(Vector3::zero());
		}
	}
};

class CameraHorizontal:public Component<CameraHorizontal>
{
public:
	const float speed = 0.001f;
public:
	virtual void SendComponentMessage(std::string message) override
	{
		if (message == "Update") Update();
	}

	void Update()
	{
		/*float horizontal = Input::Mouse::GetAccelerationX() * speed;
		this->transform.lock()->localRotation(
			this->transform.lock()->localRotation() * Quaternion::AngleAxisToRadian(horizontal,Vector3::up())
		);*/
	}
};

class Move :public Component<Move>
{
public:
	virtual void SendComponentMessage(std::string message) override
	{
		if (message == "Update") Update();
	}

	void Update() {
		/*if (Input::GetKeyPress(VK_UP))
			this->transform.lock()->position(this->transform.lock()->position() + Vector3::up()*0.5f);
		if (Input::GetKeyPress(VK_DOWN))
			this->transform.lock()->position(this->transform.lock()->position() - Vector3::up()*0.5f);
		if (Input::GetKeyPress(VK_LEFT))
			this->transform.lock()->position(this->transform.lock()->position() - Vector3::right()*0.5f);
		if (Input::GetKeyPress(VK_RIGHT))
			this->transform.lock()->position(this->transform.lock()->position() + Vector3::right()*0.5f);
		if (Input::GetKeyPress('W'))
			this->transform.lock()->localScale(this->transform.lock()->scale()*0.9f);
		if (Input::GetKeyPress('S'))
			this->transform.lock()->localScale(this->transform.lock()->scale()*1.1f);*/
	}
};