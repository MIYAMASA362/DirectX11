#pragma once

namespace DirectX
{
	/**
	@virtual
	+ Initialize
	+ Update
	+ Finalize
	+ Destroy
	+ OnDestroy
	*/
	class Behaviour :public Component
	{
	public:
		Behaviour() {};
		virtual ~Behaviour(){};
	public:
		virtual void SendBehaviourMessage(Component::Message message) override
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

		//Defualt Method
		virtual void Initialize() {};
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void Finalize() {};
		virtual void OnDestroy() {};
	};
}