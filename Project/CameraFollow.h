#pragma once

class LookAt:public MonoBehaviour<LookAt>
{
public:
	GameObject* parent;
public:
	LookAt(EntityID OwnerID) :MonoBehaviour(OwnerID) {};
	void Update()
	{
		//this->transform.lock()->LookAt(parent->transform);
	}
	/*void OnTriggerEnter(std::weak_ptr<Collider> other) override{
		if (other.lock()->gameObject.lock()->CompareTag(TagName::ChildMiku))
			this->gameObject.lock()->Destroy();
	}*/
};

class WASDMove:public MonoBehaviour<WASDMove>
{
public:
	const float speed = 0.5f;
	FieldCollider* _fieldCollider;
	bool IsGround = false;
public:
	WASDMove(EntityID OwnerID) :MonoBehaviour(OwnerID) 
	{
		this->OnDebugImGui = [this]() {
			ImGui::Checkbox("IsGround",&this->IsGround);
		};
	};
	void Update()
	{
		if (Input::GetKeyPress('W'))
			this->transform()->position(this->transform()->position() + this->transform()->forward() * speed);
		if (Input::GetKeyPress('S'))
			this->transform()->position(this->transform()->position() + this->transform()->back() *speed);
		if (Input::GetKeyPress('A'))
			this->transform()->position(this->transform()->position() + this->transform()->left() *speed);
		if (Input::GetKeyPress('D'))
			this->transform()->position(this->transform()->position() + this->transform()->right()*speed);
		if (Input::GetKeyPress('Q'))
			this->transform()->position(this->transform()->position() + this->transform()->down()*speed);
		if (Input::GetKeyPress('E'))
			this->transform()->position(this->transform()->position() + this->transform()->up()*speed);

		//this->transform()->position(Camera::ScreenToWorldPosition(Vector3::zero()));

		
		/*auto rigidbody = this->gameObject()->GetComponent<Rigidbody>();
		if (rigidbody != nullptr)
			if (Input::GetKeyTrigger(VK_SPACE))
				rigidbody->AddForce(Vector3::up() * 5.0f);*/		
	}

	void FixedUpdate()
	{
		IsGround = _fieldCollider->IsOnGround(this->transform()->position());
		if (IsGround)
		{
			Vector3 pos = this->transform()->position();
			pos.y = _fieldCollider->GetHeight(pos) + 4.0f;
			this->transform()->position(pos);
			this->gameObject()->GetComponent<Rigidbody>().lock()->SetVelocity(Vector3::zero());
		}
	}
};

class CameraHorizontal:public MonoBehaviour<CameraHorizontal>
{
public:
	const float speed = 0.001f;
public:
	CameraHorizontal(EntityID OwnerID) :MonoBehaviour(OwnerID) {}
	void Update()
	{
		/*float horizontal = Input::Mouse::GetAccelerationX() * speed;
		this->transform.lock()->localRotation(
			this->transform.lock()->localRotation() * Quaternion::AngleAxisToRadian(horizontal,Vector3::up())
		);*/
	}
};

class Move :public MonoBehaviour<Move>
{
public:
	Move(EntityID OwnerID) :MonoBehaviour(OwnerID) {};
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