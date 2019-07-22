#pragma once


class RemoveObjectMethod:public Behaviour
{
public:
	void Update() override 
	{
		if (Input::GetKeyPress(VK_SPACE))
			this->gameObject.lock()->Destroy();
	};
};