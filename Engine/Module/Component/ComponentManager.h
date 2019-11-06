#pragma once

namespace DirectX
{
	//--- ComponentManager -----------------------------------------------------------------
	class ComponentManager
	{
	private:
		static EntityComponents EntityComponentIndex;
		static std::unordered_map<ComponentTypeID, std::shared_ptr<ComponentIndex>> _ComponentTypeIndex;
	public:
		static void Create();
		static void Release();

		//ComponentTypeID ComponentTypeñàÇÃÉÜÉjÅ[ÉNÇ»ID
		static ComponentTypeID AttachComponentTypeID();

		//ComponentñàÇÃIndex
		static std::weak_ptr<ComponentIndex> GetOrCreateComponentIndex(ComponentTypeID componentTypeID);
		static void ReleaseComponentIndex(ComponentTypeID componentTypeID);

		//Message
		static void SendComponentMessage(std::string message);

		//AddComponent
		template<typename Type> static Type* AddComponent(EntityID OwnerID);

		//GetComponent
		template<typename Type> static Type* GetComponent(EntityID id);
		static std::weak_ptr<Components> GetComponents(EntityID id);

		//çÌèú
		static void DestroyComponents(EntityID id);

		//âï˙
		template<typename Type> static void ReleaseComponent(EntityID id);
		static void ReleaseComponents(EntityID id);

		//
		static void ImGui_ComponentView(EntityID id);
	};

	template<typename Type>
	inline Type* ComponentManager::AddComponent(EntityID OwnerID)
	{
		EntityComponents::iterator find = EntityComponentIndex.find(OwnerID);
		if (find == EntityComponentIndex.end())
			find = EntityComponentIndex.emplace(OwnerID, std::shared_ptr<Components>(new Components())).first;

		auto instance = new Type(OwnerID);
		ComponentTypeID id = instance->GetComponentTypeID();
		auto object = ObjectManager::GetInstance(instance->GetInstanceID()).lock();
		auto sptr = std::dynamic_pointer_cast<IComponent>(object);

		_ComponentTypeIndex.at(id)->emplace(OwnerID, sptr);
		find->second->emplace(id,sptr);

		instance->SendComponentMessage("Start");
		return instance;
	}

	template<typename Type >
	inline Type* ComponentManager::GetComponent(EntityID id)
	{
		auto component = EntityComponentIndex.at(id)->at(Component<Type>::TypeID);
		auto shared = std::dynamic_pointer_cast<Type>(component.lock());
		return shared.get();
	}

	template<typename Type>
	inline void ComponentManager::ReleaseComponent(EntityID id)
	{
		auto find = EntityComponentIndex.find(id);
		if (find == EntityComponentIndex.end()) return;
		find->second->erase(Component<Type>::TypeID);
	}
}