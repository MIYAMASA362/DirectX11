#pragma once

namespace DirectX
{
	//=== Behaviour =====================================================================
	template<typename Type>
	class Behaviour:public Component<Type>
	{	
	protected:
		bool m_IsEnable = true;
	public:
		Behaviour(EntityID OwnerID) :Component(OwnerID) {};
		virtual ~Behaviour() = default;
	public:
		void SetEnable(bool enable);
		bool GetEnable();
	public:
		virtual void OnDestroy()	override {};
		virtual void DebugImGui()	override {};
		virtual void SendComponentMessage(std::string message) {};
	};
	template<typename Type>
	inline void Behaviour<Type>::SetEnable(bool enable)
	{
		this->m_IsEnable = enable;
	}
	template<typename Type>
	inline bool Behaviour<Type>::GetEnable()
	{
		return this->m_IsEnable;
	}
}