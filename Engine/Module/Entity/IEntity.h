#pragma once

namespace DirectX
{
	class IEntity :public Object
	{
	public:
		IEntity();
		virtual ~IEntity();

		EntityID GetEntityID();

		template<typename Type> Type* AddComponent();
		template<typename Type> Type* GetComponent();
		template<typename Type> void DestroyComponent();
		void DestroyComponents();
	};

	template<typename Type> Type* IEntity::AddComponent()
	{
		return ComponentManager::AddComponent<Type>(GetEntityID());
	}

	template<typename Type> Type* IEntity::GetComponent()
	{
		return ComponentManager::GetComponent<Type>(GetEntityID());
	}

	template<typename Type> void IEntity::DestroyComponent()
	{
		ComponentManager::DestroyComponent<Type>(GetEntityID());
	}

}