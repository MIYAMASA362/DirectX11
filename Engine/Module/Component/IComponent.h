#pragma once
#include<map>
namespace DirectX
{
	typedef unsigned int ComponentID;

	//Component Interface
	class IComponent:public Object
	{
	protected:
		EntityID m_OwnerId;
		bool m_IsEnable;
	public:
		void SetEnable(bool enable);
		bool GetEnable();
		EntityID GetOwnerID();
	public:
		virtual ComponentID GetComponentID() = 0;
		virtual void Run() = 0;
		virtual void DebugImGui() = 0;
	};

	//-------------------------------------------------------------------------
	inline void IComponent::SetEnable(bool enable)
	{
		this->m_IsEnable = enable;
	}
	inline bool IComponent::GetEnable()
	{
		return this->m_IsEnable;
	}
	inline EntityID IComponent::GetOwnerID()
	{
		return this->m_OwnerId;
	}
}