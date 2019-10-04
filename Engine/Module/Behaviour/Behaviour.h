#pragma once

namespace DirectX
{
	//=== Behaviour =====================================================================
	template<typename Type>
	class Behaviour:public Component<Type>
	{
	public:
		Behaviour(EntityID OwnerID,std::string name) :Component(OwnerID,name) {};
		virtual ~Behaviour() = default;
	public:
		virtual void Run()			override {};
		virtual void OnComponent()	override {};
		virtual void OnDestroy()	override {};
		virtual void DebugImGui()	override {};
	};
}