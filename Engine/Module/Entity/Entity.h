#pragma once

namespace DirectX
{
	template<typename Type>
	class Entity:public IEntity
	{
		using EntityType = Entity<Type>;
	public:
		static std::weak_ptr<Type> GetEntity(EntityID id);

		Entity();
		virtual ~Entity();
	protected:
		static EntityIndex m_EntityIndex;
	};

	template<typename Type>
	Entity<Type>::Entity()
	{
		m_EntityIndex.emplace(GetEntityID(), EntityManager::GetEntity(GetEntityID()));
	}

	template<typename Type>
	Entity<Type>::~Entity()
	{
		m_EntityIndex.erase(GetEntityID());
	}

	template<typename Type>
	std::weak_ptr<Type> Entity<Type>::GetEntity(EntityID id)
	{
		return std::dynamic_pointer_cast<Type>(EntityManager::GetEntity(id).lock());
	}
}