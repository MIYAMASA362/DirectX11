#pragma once

class KeyMove:public Behaviour
{
public:
	GameObject* parent;
	bool IsBool = true;
public:
	void Update()
	{
		if (Input::GetKeyPress('W'))
			this->transform.lock()->position(this->transform.lock()->position() + this->transform.lock()->up());
		if (Input::GetKeyPress('S'))
			this->transform.lock()->position(this->transform.lock()->position() + this->transform.lock()->down());
		if (Input::GetKeyPress('A'))
			this->transform.lock()->rotation(this->transform.lock()->rotation() * Quaternion::Euler(Vector3::down()));
		if (Input::GetKeyPress('D'))
			this->transform.lock()->rotation(this->transform.lock()->rotation() * Quaternion::Euler(Vector3::up()));
		if (Input::GetKeyTrigger(VK_SPACE))
			if (IsBool)
			{
				this->transform.lock()->SetParent(parent); IsBool = false;
			}
			else
			{
				this->transform.lock()->DetachParent(); IsBool = true;
			}

		this->transform.lock()->LookAt(parent->transform);
	}
};

class MikuMove:public Behaviour
{
public:
	void Update()
	{
		if (Input::GetKeyPress(VK_UP))
			this->transform.lock()->position(this->transform.lock()->position() + this->transform.lock()->forward());
		if (Input::GetKeyPress(VK_DOWN))
			this->transform.lock()->position(this->transform.lock()->position() + this->transform.lock()->back());
		if (Input::GetKeyPress(VK_LEFT))
			this->transform.lock()->rotation(this->transform.lock()->rotation() * Quaternion::Euler(Vector3::down()));
		if (Input::GetKeyPress(VK_RIGHT))
			this->transform.lock()->rotation(this->transform.lock()->rotation() * Quaternion::Euler(Vector3::up()));
		
	}
};