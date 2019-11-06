#pragma once

namespace DirectX
{
	template<typename Type>
	class MonoBehaviour:public Behaviour<Type>
	{
	public:
		MonoBehaviour(EntityID OwnerID);
		virtual ~MonoBehaviour() = default;
	public:
		virtual void Start() {};
		virtual void Update(){};
		virtual void FixedUpdate(){};
	public:
		virtual void OnDestroy()	override {};

	};

	template<typename Type>
	inline MonoBehaviour<Type>::MonoBehaviour(EntityID OwnerID)
	:
		Behaviour<Type>(OwnerID)
	{
		IComponent::SendComponentMessage = [this](std::string message)
		{
			if (message == "Start") { this->Start(); return; }
			if (message == "Update") { this->Update(); return; }
			if (message == "FixedUpdate") { this->FixedUpdate(); return; }
		};
	}
}