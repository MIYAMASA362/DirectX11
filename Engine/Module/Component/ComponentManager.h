#pragma once

namespace DirectX
{
	//--- ComponentManager -----------------------------------------------------------------
	class ComponentManager
	{
	private:
		static EntityComponents _EntityComponentIndex;
		static ComponentTypeIndex _ComponentTypeIndex;
	public:
		static void Create();
		static void Release();

		//ComponentTypeID ComponentType毎のユニークなID
		static ComponentTypeID AttachComponentTypeID();

		//Component毎のIndex
		static std::weak_ptr<Components> GetOrCreateComponentIndex(ComponentTypeID componentTypeID);
		static std::weak_ptr<Components> GetComponentIndex(ComponentTypeID componentTypeID);
		
		static void ReleaseComponentIndex(ComponentTypeID componentTypeID);

		//Message
		static void SendComponentMessage(std::string message);
		static void SendComponentMessage(std::string message, EntityID entityID);

		//ComponentManagerの持つIndexに追加する
		static std::weak_ptr<IComponent> AddComponentIndex(std::weak_ptr<Object> object);

		//AddComponent
		template<typename Type> 
		static std::weak_ptr<Type> AddComponent(EntityID OwnerID);

		//GetComponent
		template<typename Type> 
		static std::weak_ptr<Type> GetComponent(EntityID id);
		template<typename Type>
		static std::list<std::weak_ptr<Type>>GetComponents(EntityID id);

		//削除
		static void DestroyComponents(EntityID id);

		//解放
		template<typename Type> static void ReleaseComponent(EntityID id);
		static void ReleaseComponents(EntityID id);

		//Debug表示
		static void ImGui_ComponentView(EntityID id);
	};

	//EntityにComponentを追加
	template<typename Type>
	inline std::weak_ptr<Type> ComponentManager::AddComponent(EntityID OwnerID)
	{
		//インスタンス生成
		auto instance = new Type(OwnerID);
		auto component = ComponentManager::AddComponentIndex(ObjectManager::GetInstance(instance->GetInstanceID()));
		return std::dynamic_pointer_cast<Type>(component.lock());
	}

	//EntityのComponentを取得
	template<typename Type >
	inline std::weak_ptr<Type> ComponentManager::GetComponent(EntityID id)
	{
		auto components = _EntityComponentIndex.at(id);
		ComponentTypeID targetID = Component<Type>::TypeID;
		for (auto component : *components)
			if (component.lock()->GetComponentTypeID() == targetID)
				return std::dynamic_pointer_cast<Type>(component.lock());
		return std::weak_ptr<Type>();
	}

	//EntityのComponentを取得
	template<typename Type>
	inline std::list<std::weak_ptr<Type>> ComponentManager::GetComponents(EntityID id)
	{
		std::list<std::weak_ptr<Type>> list;
		auto components = ComponentManager::GetComponentIndex(Component<Type>::TypeID).lock();
		for (auto component : *components)
			if (component.lock()->GetOwnerID() == id)
				list.push_back(std::dynamic_pointer_cast<Type>(component.lock()));
		return list;
	}

	//EntityのComponentを開放
	template<typename Type>
	inline void ComponentManager::ReleaseComponent(EntityID id)
	{
		auto find = _EntityComponentIndex.find(id);
		if (find == _EntityComponentIndex.end()) return;
		ComponentTypeID targetID = Component<Type>::TypeID;
		find->second->remove_if(
			[targetID](std::weak_ptr<IComponent> obj)
		{
			return obj.lock()->GetComponentTypeID() == targetID;
		});
	}
}