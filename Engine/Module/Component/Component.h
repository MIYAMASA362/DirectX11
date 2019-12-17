#pragma once

namespace DirectX
{
	//Component コンポーネント
	template<typename Type>
	class Component:public IComponent
	{
		friend class ComponentManager;
	public:
		//Component毎のID
		static const ComponentTypeID TypeID;

		Component(EntityID OwnerID);
		virtual ~Component();

		//Entityに付属されているComponentを取得
		static std::weak_ptr<Type> GetComponent(EntityID entityID);

		const ComponentTypeID GetComponentTypeID() const override final;

		virtual void OnDestroy() override {};

	protected:
		//このComponentの全インスタンス
		const std::weak_ptr<Components> _Index;
	};




	template<typename Type>
	const ComponentTypeID Component<Type>::TypeID = ComponentManager::AttachComponentTypeID();

	template<typename Type>
	inline std::weak_ptr<Type> Component<Type>::GetComponent(EntityID entityID)
	{
		auto index = ComponentManager::GetComponentIndex(TypeID);
		for (auto component : *index.lock()) {
			if (component.lock()->GetOwnerID() == entityID)
				return std::dynamic_pointer_cast<Type>(component.lock());
		}
		return std::weak_ptr<Type>();
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
		if (!_Index.expired()){
			_Index.lock()->remove_if([](std::weak_ptr<IComponent> obj) {
				return obj.expired();
			});
		}
	}

	template<typename Type>
	inline const ComponentTypeID Component<Type>::GetComponentTypeID() const
	{
		return TypeID;
	}
}