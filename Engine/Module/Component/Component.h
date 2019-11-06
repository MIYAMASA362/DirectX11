#pragma once

namespace DirectX
{
	template<typename Type>
	class Component:public IComponent
	{
		friend class ComponentManager;
	public:
		static const ComponentTypeID TypeID;

	protected:
		const std::shared_ptr<ComponentIndex> _Index;
	public:
		Component(EntityID OwnerID);
		virtual ~Component();

		static std::weak_ptr<Type> GetComponent(EntityID entityID);

		virtual const ComponentTypeID GetComponentTypeID() const override final;

		virtual void OnDestroy() override {};
	};

	template<typename Type>
	const ComponentTypeID Component<Type>::TypeID = ComponentManager::AttachComponentTypeID();

	template<typename Type>
	inline std::weak_ptr<Type> Component<Type>::GetComponent(EntityID entityID)
	{
		return std::dynamic_pointer_cast<Type>(ComponentManager::GetOrCreateComponentIndex(TypeID).lock()->at(entityID).lock());
	}

	template<typename Type>
	inline Component<Type>::Component(EntityID OwnerID)
	:
		IComponent(OwnerID),
		_Index(ComponentManager::GetOrCreateComponentIndex(TypeID))
	{
		this->SendComponentMessage = [](std::string message)
		{
			
		};

		this->OnDebugImGui = [this]()
		{
			if (ImGui::TreeNode(typeid(*this).name()))
			{
				ImGui::Text(("ID:" + std::to_string(this->GetInstanceID())).c_str());
				ImGui::TreePop();
			}
		};
	}

	template<typename Type>
	inline Component<Type>::~Component()
	{
		_Index->erase(this->GetOwnerID());
	}

	template<typename Type>
	inline const ComponentTypeID Component<Type>::GetComponentTypeID() const
	{
		return TypeID;
	}
}