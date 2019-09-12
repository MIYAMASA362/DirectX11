#pragma once

#include"Module\Behaviour\Behaviour.h"

namespace DirectX
{
	template<typename Type>
	class MonoBehaviour:public Behaviour<Type>
	{
	public:
		MonoBehaviour(std::string name) :Behaviour(name) {};
		virtual ~MonoBehaviour() = default;
	public:
		virtual void Initialize() override {};
		virtual void Update() override {};
		virtual void FixedUpdate() override {};
		virtual void Render() override {};
		virtual void Finalize() override {};
	public:
		virtual void Run()			override {};
		virtual void OnComponent()	override {};
		virtual void OnDestroy()	override {};
		virtual void DebugImGui()	override {};
	};
}