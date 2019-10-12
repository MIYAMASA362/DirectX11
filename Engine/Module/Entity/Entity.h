#pragma once

namespace DirectX
{
	//=== Entity ==============================================================
	template<typename Type>
	class Entity:public IEntity
	{
	protected:
		static EntityList m_EntityIndex;
	public:
		static std::weak_ptr<Type> CreateEntity(Type* instance);
		static std::weak_ptr<Type> GetEntity(EntityID id);
	public:
		Entity();
		virtual ~Entity();
	protected:
		void AddIndex(Type* instance);
	};
	//-------------------------------------------------------------------------

	template<typename Type>
	inline std::weak_ptr<Type> Entity<Type>::CreateEntity(Type* instance)
	{
		auto add = EntityManager::CreateEntity<Type>(instance);
		m_EntityIndex.push_back(add->GetEntityID());
		return add;
	}

	template<typename Type>
	inline std::weak_ptr<Type> Entity<Type>::GetEntity(EntityID id)
	{
		return std::static_pointer_cast<Type>(EntityManager::GetEntity(id).lock());
	}

	template<typename Type>
	inline Entity<Type>::Entity()
	{
		
	}

	template<typename Type>
	inline Entity<Type>::~Entity()
	{
		m_EntityIndex.remove(m_EntityID);
	}

	template<typename Type>
	inline void Entity<Type>::AddIndex(Type* instance)
	{
		m_EntityIndex.push_back(instance->GetEntityID());
		EntityManager::CreateEntity<IEntity>(instance);
	}
}