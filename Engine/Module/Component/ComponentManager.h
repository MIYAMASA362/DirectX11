#pragma once

namespace DirectX
{
	//--- ComponentManager -----------------------------------------------------------------
	class ComponentManager
	{
	private:
		static EntityComponents EntityComponentIndex;
	public:
		static void Create();
		static void Release();

		//ComponentTypeID ComponentType–ˆ‚Ìƒ†ƒj[ƒN‚ÈID
		static ComponentTypeID AttachComponentTypeID();

		//Message
		static void SendComponentMessage(std::string message);

		//AddComponent
		template<typename Type> static Type* AddComponent(EntityID OwnerID);

		//GetComponent
		template<typename Type> static Type* GetComponent(EntityID id);
		static std::weak_ptr<Components> GetComponents(EntityID id);

		//íœ
		static void DestroyComponents(EntityID id);

		//‰ğ•ú
		template<typename Type> static void ReleaseComponent(EntityID id);
		static void ReleaseComponents(EntityID id);
	};

	//--------------------------------------------------------------------------------------
	template<typename Type>
	inline Type* ComponentManager::AddComponent(EntityID OwnerID)
	{
		EntityComponents::iterator find = EntityComponentIndex.find(OwnerID);
		if (find == EntityComponentIndex.end())
		{
			EntityComponentIndex.emplace(OwnerID, std::shared_ptr<Components>(new Components()));
			find = EntityComponentIndex.find(OwnerID);
		}
		//Componentİ’è
		auto instance =  new Type(OwnerID);
		instance->AddComponent();
		auto add = Type::GetComponent(OwnerID).lock();
		IComponent* icomponent = add.get();
		find->second->emplace(icomponent->GetComponentTypeID(),add);
		icomponent->SendComponentMessage("Start");
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