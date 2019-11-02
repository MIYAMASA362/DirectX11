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
public:
	WASDMove(EntityID OwnerID) :MonoBehaviour(OwnerID) {};
	void Update()
	{
		/*if (Input::GetKeyPress('W'))
			this->transform().lock()->position(this->transform().lock()->position() + this->transform().lock()->forward() * speed);
		if (Input::GetKeyPress('S'))
			this->transform().lock()->position(this->transform().lock()->position() + this->transform().lock()->back()*speed);
		if (Input::GetKeyPress('A'))
			this->transform().lock()->position(this->transform().lock()->position() + this->transform().lock()->left() *speed);
		if (Input::GetKeyPress('D'))
			this->transform().lock()->position(this->transform.lock()->position() + this->transform.lock()->right()*speed);
		auto rigidbody = this->gameObject().lock()->GetComponent<Rigidbody>().lock().get();
		if (rigidbody != nullptr)
			if (Input::GetKeyTrigger(VK_SPACE))
				rigidbody->AddForce(Vector3::up() * 5.0f);*/
				
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