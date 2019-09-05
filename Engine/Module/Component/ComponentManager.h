#pragma once

namespace DirectX
{
	template<typename Type>
	class IComponent
	{
	protected:
		std::map<EntityID, std::shared_ptr<Type>> ComponentIndex;
	protected:
		void AddComponent(EntityID id);
		void RemoveComponent(EntityID id);
	};

	//-------------------------------------------------------------------------
	template<typename Type>
	inline void IComponent<Type>::AddComponent(EntityID id)
	{
		ComponentIndex.emplace(id,std::shared_ptr<Type>(new Type()));
	}
	template<typename Type>
	inline void IComponent<Type>::RemoveComponent(EntityID id)
	{
		ComponentIndex.erase(id);
	}
}