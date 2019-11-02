#pragma once


class RemoveObjectMethod:public MonoBehaviour<RemoveObjectMethod>
{
public:
	RemoveObjectMethod(EntityID OwnerID) :MonoBehaviour(OwnerID) { };
public:
	void Update() override 
	{
		/*if (Input::GetKeyPress(VK_SPACE))
			this->gameObject.lock()->Destroy();*/
	};
};