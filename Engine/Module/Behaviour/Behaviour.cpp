#include<string>
#include<memory>

#include"Module\Object\Object.h"
#include"Module\Component\Component.h"
#include "Behaviour.h"

void DirectX::Behaviour::SendBehaviourMessage(Component::Message message)
{
	switch (message)
	{
	case Component::Initialize:
		Initialize();
		break;
	case Component::Update:
		Update();
		break;
	case Component::Finalize:
		Finalize();
		break;
	case Component::FixedUpdate:
		FixedUpdate();
		break;
	default:
		break;
	}
};