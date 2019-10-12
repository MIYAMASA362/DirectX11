#pragma once

namespace DirectX
{
	//Entity Interface
	class IEntity :public Object
	{
	protected:
		const EntityID m_EntityID;
		bool m_IsEnable;
	public:
		IEntity();
		virtual ~IEntity();
		const EntityID GetEntityID();
		bool GetIsEnable();
	public:
		template<typename Type> Type* AddComponent();
		template<typename Type> std::weak_ptr<Type> GetComponent();
		template<typename Type> void DestroyComponent();
	};

	template<typename Type> Type* IEntity::AddComponent()
	{
		return ComponentManager::AddComponent<Type>(this->m_EntityID);
	}
	template<typename Type> std::weak_ptr<Type> IEntity::GetComponent()
	{
		return ComponentManager::GetComponent<Type>(this->m_EntityID);
	}
	template<typename Type>
	inline void IEntity::DestroyComponent()
	{
		ComponentManager::DestroyComponent<Type>(this->m_EntityID);
	}
}