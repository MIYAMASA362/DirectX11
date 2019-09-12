#pragma once

namespace DirectX
{
	//=== Behaviour =====================================================================
	template<typename Type>
	class Behaviour:public Component<Type>
	{
	protected:
		bool m_IsEnable;
	public:
		Behaviour(std::string name) :Component(name) {};
		virtual ~Behaviour() = default;
	public:
		bool GetEnable();
		void SetEnable(bool IsEnable);
	public:
		virtual void Run()			override {};
		virtual void OnComponent()	override {};
		virtual void OnDestroy()	override {};
		virtual void DebugImGui()	override {};
	};

	//-----------------------------------------------------------------------------------
	template<typename Type>
	inline bool Behaviour<Type>::GetEnable()
	{
		return this->m_IsEnable;
	}
	template<typename Type>
	inline void Behaviour<Type>::SetEnable(bool IsEnable)
	{
		this->m_IsEnable = IsEnable;
	}
}