#pragma once

namespace DirectX
{
	using ComponentID = unsigned int;
	using EntityID = unsigned int;

	template<typename Type>
	class Component:public IComponent
	{
	protected:
		static const ComponentID m_id;
		static std::map<EntityID, std::weak_ptr<Type>> ComponentIndex;
	public:
		static ComponentID GetID();
		static void AddComponent(std::weak_ptr<Type> instance);
		static std::weak_ptr<Type> GetComponent(EntityID id);
		static void DestroyComponent(EntityID id);
	protected:
		const std::string name;
	public:
		Component(EntityID OwnerID,std::string name);
		virtual ~Component();
	public:
		virtual void Run() override {};
		virtual void OnComponent() {};
		virtual void OnDestroy() override {};
		virtual void DebugImGui() override {};
		virtual void AddComponentIndex(std::weak_ptr<Type> instance);	//ComponentIndex�ւ̓���ϊ�
	public:
		ComponentID GetComponentID()override final;
	};

	//----------------------------------------------------------------------------
	template<typename Type>
	const ComponentID Component<Type>::m_id = ComponentManager::CreateComponent<Type>();

	template<typename Type>
	inline void Component<Type>::AddComponent(std::weak_ptr<Type> instance)
	{
		instance.lock()->AddComponentIndex(instance);
		instance.lock()->OnComponent();
	}
	template<typename Type>
	inline std::weak_ptr<Type> Component<Type>::GetComponent(EntityID id)
	{
		return ComponentIndex.at(id);
	}

	template<typename Type> 
	inline void Component<Type>::DestroyComponent(EntityID id)
	{
		ComponentIndex.at(id).lock()->OnDestroy();
		ComponentIndex.erase(id);
	}

	template<typename Type>
	inline ComponentID Component<Type>::GetID()
	{
		return m_id;
	}
	template<typename Type>
	inline void Component<Type>::AddComponentIndex(std::weak_ptr<Type> instance)
	{
		ComponentIndex.emplace(instance.lock()->GetOwnerID(), instance);
	}
	template<typename Type>
	inline ComponentID Component<Type>::GetComponentID()
	{
		return GetID();
	}

	template<typename Type>
	inline Component<Type>::Component(EntityID OwnerID, std::string name)
	:
		IComponent(OwnerID),
		name(name)
	{

	}

	template<typename Type>
	inline Component<Type>::~Component()
	{
		ComponentIndex.erase(m_OwnerId);
	}
}