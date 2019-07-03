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
			this->transform.lock()->rotation(transform.lock()->rotation() * Quaternion::Euler(Vector3::up()).conjugate());
		if (Input::GetKeyPress('D'))
			this->transform.lock()->rotation(this->transform.lock()->rotation() * Quaternion::Euler(Vector3::up()));

		XMMATRIX matrix = this->transform.lock()->MatrixScaling();
		matrix = matrix;
	}
};

class MikuMove:public Behaviour
{
public:
	void Update()
	{
		if (Input::GetKeyPress(VK_UP))
			this->transform.lock()->scale(this->transform.lock()->scale()*1.2f);
		if (Input::GetKeyPress(VK_DOWN))
			this->transform.lock()->scale(this->transform.lock()->scale()*0.8f);
		if (Input::GetKeyPress(VK_LEFT))
			this->transform.lock()->rotation(transform.lock()->rotation() * Quaternion::Euler(Vector3::up()).conjugate());
		if (Input::GetKeyPress(VK_RIGHT))
			this->transform.lock()->rotation(this->transform.lock()->rotation() * Quaternion::Euler(Vector3::up()));

		XMMATRIX matrix = this->transform.lock()->MatrixScaling();
		matrix = matrix;
	}
};