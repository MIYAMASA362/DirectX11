#pragma once

namespace DirectX
{
	class EntityManager
	{
		friend IEntity;
	public:
		static EntityIndex m_EntityIndex;

		static void Create();
		static void Release();

		static std::weak_ptr<IEntity> CreateEntity(IEntity* instance);
		static std::weak_ptr<IEntity> GetEntity(EntityID id);
		static void ReleaseEntity(EntityID id);
	};
}