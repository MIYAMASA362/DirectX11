#pragma once

#include<memory>
#include<map>
#include<vector>

namespace DirectX
{
	using ComponentID = unsigned int;
	using EntityID = unsigned int;

	using Components = std::map<ComponentID, std::weak_ptr<IComponent>>;
	using EntityComponents = std::map<EntityID, std::shared_ptr<Components>>;

	//--- ComponentManager -----------------------------------------------------------------
	class ComponentManager
	{
	private:
		static ComponentID m_id;
		static EntityComponents EntityComponentIndex;
	public:
		static void Create();
		static void Release();
	public:
		template<typename Type> static Type* AddComponent(EntityID id);
		template<typename Type> static std::weak_ptr<Type> GetComponent(EntityID id);
		static void DestroyComponents(EntityID id);
		template<typename Type> static void DestroyComponent(EntityID id);
		template<typename Type> static ComponentID CreateComponent();
	};

	//--------------------------------------------------------------------------------------
	inline void ComponentManager::Create(){
		EntityComponentIndex.clear();
	}
	inline void ComponentManager::Release(){
		EntityComponentIndex.clear();
	}
	inline void ComponentManager::DestroyComponents(EntityID id)
	{
		EntityComponentIndex.at(id)->clear();
	}
	template<typename Type>
	inline Type* ComponentManager::AddComponent(EntityID id)
	{
		auto add = new Type();

		auto components = std::shared_ptr<Components>();
		try{
			//id‚ÌComponentsƒŠƒXƒg‚ª‘¶İ‚·‚é‚Ì‚©
			components = EntityComponentIndex.at(id);
		}
		catch (const std::out_of_range&){
			//‘¶İ‚µ‚Ä‚¢‚È‚¢
			components = std::shared_ptr<Components>();
			EntityComponentIndex.emplace(id,components);
		}
		components->emplace(add->GetComponentID(),add);
		add->m_OwnerId = id;
		return add;
	}
	template<typename Type>
	inline std::weak_ptr<Type> ComponentManager::GetComponent(EntityID id)
	{
		return std::static_pointer_cast<Type>(EntityComponentIndex.at(id)->at(Type::GetID()).lock());
	}
	template<typename Type>
	inline void ComponentManager::DestroyComponent(EntityID id)
	{
		auto components = EntityComponentIndex.at(id);
		components->erase(Type::GetID());
		return;
	}
	template<typename Type>
	inline ComponentID ComponentManager::CreateComponent()
	{
		ComponentID n = m_id;
		m_id++;
		return n;
	}
}