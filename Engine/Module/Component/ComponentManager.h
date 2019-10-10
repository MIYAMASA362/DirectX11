#pragma once

namespace DirectX
{
	//--- ComponentManager -----------------------------------------------------------------
	class ComponentManager
	{
	private:
		static std::vector<ComponentID> IdIndex;
		static EntityComponents EntityComponentIndex;
	public:
		static void Create();
		static void Release();
	public:
		template<typename Type> static Type* AddComponent(EntityID id);
		template<typename Type> static std::weak_ptr<Type> GetComponent(EntityID id);
		static std::weak_ptr<Components> GetComponents(EntityID id);
		static void DestroyComponents(EntityID id);
		template<typename Type> static void DestroyComponent(EntityID id);
		static ComponentID CreateComponent();
	public:
		static void SendComponentMessage(std::string message);
	};

	//--------------------------------------------------------------------------------------
	inline void ComponentManager::Create(){
		EntityComponentIndex.clear();
	}
	inline void ComponentManager::Release(){
		EntityComponentIndex.clear();
	}
	inline std::weak_ptr<Components> ComponentManager::GetComponents(EntityID id)
	{
		if (EntityComponentIndex.size() == 0)
			return std::weak_ptr<Components>();
		return EntityComponentIndex.at(id);
	}
	template<typename Type>
	inline Type* ComponentManager::AddComponent(EntityID id)
	{
		//idÇÃComponentsÉäÉXÉgÇ™ë∂ç›Ç∑ÇÈÇÃÇ©
		try {
			EntityComponentIndex.at(id);
		}
		//ë∂ç›ÇµÇƒÇ¢Ç»Ç¢
		catch (const std::out_of_range&) {
			EntityComponentIndex.emplace(id, std::shared_ptr<Components>(new Components()));
		}
		//Componentê›íË
		auto add = std::shared_ptr<Type>(new Type(id));
		EntityComponentIndex.at(id)->emplace(Component<Type>::GetID(),add);
		add->AddComponentIndex(add);
		add->OnComponent();
		add->SendComponentMessage("Start");
		return add.get();
	}
	template<typename Type>
	inline std::weak_ptr<Type> ComponentManager::GetComponent(EntityID id)
	{
		return std::static_pointer_cast<Type>(EntityComponentIndex.at(id)->at(Component<Type>::GetID()));
	}
	template<typename Type>
	inline void ComponentManager::DestroyComponent(EntityID id)
	{
		Component<Type>::DestroyComponent(id);
		EntityComponentIndex.at(id)->erase(Component<Type>::GetID());
		return;
	}
}