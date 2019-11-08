#pragma once

class BallScript:public MonoBehaviour<BallScript>
{
	float speed = 0.5f;
	float rot = 0.2f;
public:
	BallScript(EntityID OwnerID) :MonoBehaviour(OwnerID) {};
public:
	void Update() override
	{
		if (Input::GetKeyPress('W'))
		{
			this->transform()->position(this->transform()->position() + Vector3::forward() * speed);
			this->transform()->rotation(this->transform()->rotation()*Quaternion::AngleAxisToRadian(rot,Vector3::right()));
		}

		if (Input::GetKeyPress('S'))
		{
			this->transform()->position(this->transform()->position() + Vector3::back() * speed);
			this->transform()->rotation(this->transform()->rotation()*Quaternion::AngleAxisToRadian(rot, -Vector3::right()));
		}

		if (Input::GetKeyPress('A'))
		{
			this->transform()->position(this->transform()->position() + Vector3::left() * speed);
			this->transform()->rotation(this->transform()->rotation() * Quaternion::AngleAxisToRadian(rot,-Vector3::forward()));
		}

		if (Input::GetKeyPress('D'))
		{
			this->transform()->position(this->transform()->position() + Vector3::right() * speed);
			this->transform()->rotation(this->transform()->rotation() * Quaternion::AngleAxisToRadian(rot,Vector3::forward()));
		}
	}

	void DebugImGui()
	{
		if (ImGui::TreeNode("BallScript"))
		{
			ImGui::InputFloat("Speed", &speed);
			ImGui::InputFloat("rot", &rot);
			ImGui::TreePop();
		}
	}

};