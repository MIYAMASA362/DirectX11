#pragma once

class CameraMouse:public MonoBehaviour<CameraMouse>
{
public:
	CameraMouse(EntityID OwnerID) :MonoBehaviour(OwnerID) {};
public:
	const float MouseSpeed = 0.001f;
	std::shared_ptr<Transform>looktarget;
public:
	void Update() override
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

	void OnDestroy()  override
	{
		OutputDebugStringA("Destroy this Component\n");
	}
};
