#pragma once

namespace DirectX
{
	template<typename Type>
	class MonoBehaviour:public Behaviour<Type>
	{
	public:
		MonoBehaviour(EntityID OwnerID,std::string name);
		virtual ~MonoBehaviour() = default;
	public:
		virtual void SendComponentMessage(std::string message) override;
	public:
		virtual void Start() {};
		virtual void Update(){};
		virtual void FixedUpdate(){};
	public:
		virtual void OnComponent()	override {};
		virtual void AddComponentIndex(std::weak_ptr<Type> instance) override;
		virtual void OnDestroy()	override {};
		virtual void DebugImGui()	override {};
	};

	template<typename Type>
	inline MonoBehaviour<Type>::MonoBehaviour(EntityID OwnerID, std::string name)
	:
		Behaviour<Type>(OwnerID,name)
	{
		
	}
	template<typename Type>
	inline void MonoBehaviour<Type>::SendComponentMessage(std::string message)
	{
		if (message == "Start") { Start(); return; }
		if (message == "Update") { Update(); return; }
		if (message == "FixedUpdate"){ FixedUpdate(); return; }
	}
	template<typename Type>
	inline void MonoBehaviour<Type>::AddComponentIndex(std::weak_ptr<Type> instance)
	{
		Behaviour<Type>::AddComponentIndex(instance);
	}
}