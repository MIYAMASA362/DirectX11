#pragma once

class CameraMouse:public Component<CameraMouse>
{
public:
	const float MouseSpeed = 0.001f;
	std::shared_ptr<Transform>looktarget;
public:

	virtual void SendComponentMessage(std::string message)
	{
		if(message == "Update")
		{
			Update();
		}
	}

	void Update()
	{
		//float horizontal = 0.0f, vertical = 0.0f;

		//vertical = Input::Mouse::GetAccelerationY() * MouseSpeed;

		//this->transform()->localRotation(
		//	this->transform()->localRotation() * Quaternion::AngleAxisToRadian(vertical,Vector3::right())
		//);
		////this->transform.lock()->LookAt(looktarget);

		if (Input::GetKeyTrigger('Q'))
			gameObject()->Destroy();
	}
};
