#pragma once

namespace DirectX
{
	//=== EntityManager ==========================================================
	class EntityManager
	{
		friend IEntity;
	public:
		static EntityIndex m_EntityIndex;
		static EntityID AttachEntityID();
	public:
		static void Create();
		static void Release();
	public:
		template<typename Type>
		static std::weak_ptr<Type> CreateEntity(Type* instance);
		static std::weak_ptr<IEntity> GetEntity(EntityID id);
		static void RemoveEntity(EntityID id);
		static void IndexClearnUp();
	};

	//----------------------------------------------------------------------------
	
	template<typename Type>
	inline std::weak_ptr<Type> EntityManager::CreateEntity(Type* instance)
	{
		auto add = std::shared_ptr<Type>(instance);
		m_EntityIndex.emplace(add->GetEntityID(),add);
		return add;
	}
}