#pragma once

#include"Module\Behaviour\Behaviour.h"

namespace DirectX
{
	template<typename Type>
	class MonoBehaviour:public Behaviour<Type>
	{
	public:
		MonoBehaviour(EntityID OwnerID,std::string name) :Behaviour(OwnerID,name) {};
		virtual ~MonoBehaviour() = default;
	public:
		virtual void Initialize(){};
		virtual void Update(){};
		virtual void FixedUpdate(){};
		virtual void Render(){};
		virtual void Finalize(){};
	public:
		virtual void Run()			override {};
		virtual void OnComponent()	override {};
		virtual void OnDestroy()	override {};
		virtual void DebugImGui()	override {};
	};
}