#pragma once

class CameraMouse:public Behaviour
{
public:
	void Update() override
	{
		this->transform.lock()->rotation(
			this->transform.lock()->rotation()*(this->transform.lock()->up()*Input::Mouse::GetAccelerationX()* 0.0001f)
		);
	}
};
