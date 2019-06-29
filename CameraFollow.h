#pragma once

class KeyMove:public Behaviour
{
public:
	void Update()
	{
		if (Input::GetKeyPress('W'))
			this->transform.lock()->position(this->transform.lock()->position() + this->transform.lock()->forward());
		if (Input::GetKeyPress('S'))
			this->transform.lock()->position(this->transform.lock()->position() + this->transform.lock()->back());
		if (Input::GetKeyPress('A'))
			this->transform.lock()->rotation(transform.lock()->rotation() * Quaternion::Euler(Vector3(0.0f,-1.0f,0.0f)).conjugate());
		if (Input::GetKeyPress('D'))
			this->transform.lock()->rotation(this->transform.lock()->rotation() * Quaternion::Euler(Vector3::up()));
	}
};