#pragma once


class RemoveObjectMethod:public MonoBehaviour<RemoveObjectMethod>
{
public:
	RemoveObjectMethod() :MonoBehaviour("RemoveObjectMethod") { };
public:
	void Update() override 
	{
		/*if (Input::GetKeyPress(VK_SPACE))
			this->gameObject.lock()->Destroy();*/
	};
};