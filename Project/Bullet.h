#pragma once

class Bullet:public Component<Bullet>
{
public:
	const float speed = 0.5f;

	virtual void SendComponentMessage(std::string message) override
	{
		if (message == "Update") Update();
	}
	void Update()
	{
		//this->transform.lock()->position(this->transform.lock()->position()+this->transform.lock()->forward() * speed);
	}
};

class Shot:public Component<Shot>
{
public:

};

class RotationBox:public Component<RotationBox>
{
public :
	virtual void SendComponentMessage(std::string message) override
	{
		if (message == "Update") Update();
	}

	void Update()
	{
		Quaternion q = this->gameObject()->transform().lock()->rotation();
		q *= Quaternion::AngleAxisToEuler(0.1f,Vector3::up());
		this->gameObject()->transform().lock()->rotation(q);
	}
};