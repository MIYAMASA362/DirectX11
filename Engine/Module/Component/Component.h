#pragma once

namespace DirectX
{
	template<typename Type>
	class Component:public IComponent
	{
		friend class ComponentManager;

		using ComponentType  = Component<Type>;
		using ComponentIndex = std::unordered_map<EntityID, std::weak_ptr<Type>>;

	public:
		static const ComponentTypeID TypeID;

	protected:
		static ComponentIndex Index;

	private:
		void AddComponent();

	public:
		Component(EntityID OwnerID);
		virtual ~Component();

		static std::weak_ptr<Type> GetComponent(EntityID entityID);

		virtual const ComponentTypeID GetComponentTypeID() const override final;

		virtual void OnDestroy() override {};
		virtual void DebugImGui() override {};
		virtual void SendComponentMessage(std::string message) override {};

	};

	//----------------------------------------------------------------------------
	template<typename Type>
	const ComponentTypeID Component<Type>::TypeID = ComponentManager::AttachComponentTypeID();

	template<typename Type>
	inline void Component<Type>::AddComponent()
	{
		auto object = ObjectManager::GetInstance(this->GetInstanceID()).lock();
		Index.emplace(this->GetOwnerID(), std::dynamic_pointer_cast<Type>(object));
	}

	template<typename Type>
	inline std::weak_ptr<Type> Component<Type>::GetComponent(EntityID entityID)
	{
		return Index.at(entityID);
	}

	template<typename Type>
	inline Component<Type>::Component(EntityID OwnerID)
	:
		IComponent(OwnerID)
	{
		
	}

	template<typename Type>
	inline Component<Type>::~Component()
	{
		Index.erase(this->GetOwnerID());
	}

	template<typename Type>
	inline const ComponentTypeID Component<Type>::GetComponentTypeID() const
	{
		return TypeID;
	}
}