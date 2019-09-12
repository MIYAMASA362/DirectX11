#pragma once

#include<memory>
#include<map>
namespace DirectX
{
	using EntityID = unsigned int;
	class IEntity;

	//=== Entity ==============================================================
	template<typename Type>
	class Entity:public IEntity
	{
	protected:
		static std::map<EntityID, std::shared_ptr<Type>> m_EntityIndex;
	public:
		static std::weak_ptr<Type> CreateEntity();
		static std::weak_ptr<Type> GetEntity(EntityID id);
		static void RemoveEntity(EntityID id);
	public:
		Entity();
		virtual ~Entity();
	};
	//-------------------------------------------------------------------------

	template<typename Type>
	inline std::weak_ptr<Type> Entity<Type>::CreateEntity()
	{
		auto add = std::shared_ptr<Type>(new Type());
		m_EntityIndex.emplace(add->GetEntityID(),add);
		return add;
	}

	template<typename Type>
	inline std::weak_ptr<Type> Entity<Type>::GetEntity(EntityID id)
	{
		return m_EntityIndex.at(id);
	}

	template<typename Type>
	inline void Entity<Type>::RemoveEntity(EntityID id)
	{
		m_EntityIndex.erase(id);
	}
	
	template<typename Type>
	inline Entity<Type>::Entity()
	{
		
	}
	template<typename Type>
	inline Entity<Type>::~Entity()
	{

	}
}