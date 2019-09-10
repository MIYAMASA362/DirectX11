#pragma once

#include<memory>
#include<map>
#include<vector>
namespace DirectX
{
	typedef unsigned int ComponentID;
	typedef unsigned int EntityID;

	class Transform;

	using Components = std::map<ComponentID, std::weak_ptr<IComponent>>;
	using EntityComponents = std::map<EntityID, std::shared_ptr<Components>>;
	
	//--- ComponentManager -----------------------------------------------------------------
	class ComponentManager
	{
	private:
		ComponentID m_id;
		EntityComponents EntityComponentIndex;	//Entity‚ªŠ‚µ‚Ä‚¢‚éComponent‚Ì•ÛŠÇŒÉ
	public:
		void Create();
		void Release();
	public:
		template<typename Type> Type* AddComponent(EntityID id);
		void DestroyComponents(EntityID id);
		template<typename Type> void DestroyComponent(EntityID id);
		template<typename Type> void CreateComponent();
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
			components = EntityComponentIndex.at(id);
		}
		catch (const std::out_of_range&){
			components = std::shared_ptr<Components>();
			EntityComponentIndex.emplace(id,components);
		}
		components->emplace(add->GetComponentID(),add->AddComponent(id));
		return add;
	}
	template<typename Type>
	inline void ComponentManager::DestroyComponent(EntityID id)
	{
		auto components = EntityComponentIndex.at(id);
		components->erase(Type::GetID());
		return;
	}
	template<typename Type>
	inline void ComponentManager::CreateComponent()
	{
		int n = m_id;
		m_id++;
		return n;
	}
}