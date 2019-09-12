#pragma once

namespace DirectX
{
	using EntityID = unsigned int;
	class IEntity;

	//=== EntityManager ==========================================================
	class EntityManager
	{
		friend IEntity;
	private:
		static EntityID m_EntityID;
		static std::map<EntityID, std::weak_ptr<IEntity>> EntityIndex;
		static EntityID AttachEntityID();
	public:
		static void Create();
		static void Release();
	public:
		template<typename Type>
		static Type* CreateEntity(Type* instance);
		static void RemoveEntity(EntityID id);
		static std::weak_ptr<IEntity> GetEntity(EntityID id);
	};

	//----------------------------------------------------------------------------
	
	template<typename Type>
	inline Type* EntityManager::CreateEntity(Type* instance)
	{
		auto add = std::weak_ptr<IEntity>(instance);
		EntityIndex.emplace(add.lock()->GetEntityID(),add);
		return instance;
	}
	
}