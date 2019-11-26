#pragma once

namespace DirectX
{
	class IEntity :public Object
	{
	private:
		std::weak_ptr<IEntity> _self;
	public:
		IEntity();
		virtual ~IEntity();

		EntityID GetEntityID();
		std::weak_ptr<IEntity> GetEntity();

		template<typename Type> std::weak_ptr<Type> AddComponent();
		template<typename Type> std::weak_ptr<Type> GetComponent();
		template<typename Type> std::list<std::weak_ptr<Type>> GetComponents();
		template<typename Type> void DestroyComponent();
		void DestroyComponents();

		void Destroy() override;
	};

	template<typename Type> std::weak_ptr<Type> IEntity::AddComponent()
	{
		return ComponentManager::AddComponent<Type>(GetEntityID());
	}

	template<typename Type> std::weak_ptr<Type> IEntity::GetComponent()
	{
		return ComponentManager::GetComponent<Type>(GetEntityID()).lock();
	}

	template<typename Type>
	inline std::list<std::weak_ptr<Type>> IEntity::GetComponents()
	{
		return ComponentManager::GetComponents<Type>(GetEntityID());
	}

	template<typename Type> void IEntity::DestroyComponent()
	{
		ComponentManager::DestroyComponent<Type>(GetEntityID());
	}

}