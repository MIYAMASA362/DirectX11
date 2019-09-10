#pragma once

#include<map>
#include<memory>
#include"Module\Object\Object.h"
#include"Module\Entity\IEntity.h"

namespace DirectX
{
	class GameObject;
	//--- EntityManager ----------------------------------------------------------
	template<typename Type>
	class EntityManager{
	private:
		EntityID m_EntityID = 0;
		std::map<EntityID, std::shared_ptr<Type>> EntityIndex;
	public:
		void Create();
		void Release();
	public:
		std::weak_ptr<Type> CreateEntity(Type* instance);
		void RemoveEntity(EntityID id);
		std::weak_ptr<Type> GetEntity(EntityID id);
		EntityID AttachEntityID();
	};

	//----------------------------------------------------------------------------
	template<typename Type>
	inline void EntityManager<Type>::Create()
	{
		EntityIndex.clear();
	}
	template<typename Type>
	inline void EntityManager<Type>::Release()
	{
		EntityIndex.clear();
	}
	template<typename Type>
	inline std::weak_ptr<Type> EntityManager<Type>::CreateEntity(Type* instance)
	{
		auto add = std::shared_ptr<Type>(instance);
		EntityIndex.emplace(instance->GetEntityID(),add);
		return add;
	}
	template<typename Type>
	inline void EntityManager<Type>::RemoveEntity(EntityID id)
	{
		EntityIndex.erase(id);
	}
	template<typename Type>
	inline std::weak_ptr<Type> EntityManager<Type>::GetEntity(EntityID id)
	{
		return EntityIndex.at(id);
	}
	template<typename Type>
	inline EntityID EntityManager<Type>::AttachEntityID()
	{
		EntityID id = this->m_EntityID;
		this->m_EntityID++;
		return id;
	}
}