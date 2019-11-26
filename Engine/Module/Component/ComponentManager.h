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

		//ComponentTypeID ComponentType���̃��j�[�N��ID
		static ComponentTypeID AttachComponentTypeID();

		//Component����Index
		static std::weak_ptr<Components> GetOrCreateComponentIndex(ComponentTypeID componentTypeID);
		static std::weak_ptr<Components> GetComponentIndex(ComponentTypeID componentTypeID);
		
		static void ReleaseComponentIndex(ComponentTypeID componentTypeID);

		//Message
		static void SendComponentMessage(std::string message);
		static void SendComponentMessage(std::string message, EntityID entityID);

		//ComponentManager�̎���Index�ɒǉ�����
		static std::weak_ptr<IComponent> AddComponentIndex(std::weak_ptr<Object> object);

		//AddComponent
		template<typename Type> 
		static std::weak_ptr<Type> AddComponent(EntityID OwnerID);

		//GetComponent
		template<typename Type> 
		static std::weak_ptr<Type> GetComponent(EntityID id);
		template<typename Type>
		static std::list<std::weak_ptr<Type>>GetComponents(EntityID id);

		//�폜
		static void DestroyComponents(EntityID id);

		//���
		template<typename Type> static void ReleaseComponent(EntityID id);
		static void ReleaseComponents(EntityID id);

		//Debug�\��
		static void ImGui_ComponentView(EntityID id);
	};

	//Entity��Component��ǉ�
	template<typename Type>
	inline std::weak_ptr<Type> ComponentManager::AddComponent(EntityID OwnerID)
	{
		//�C���X�^���X����
		auto instance = new Type(OwnerID);
		auto component = ComponentManager::AddComponentIndex(ObjectManager::GetInstance(instance->GetInstanceID()));
		return std::dynamic_pointer_cast<Type>(component.lock());
	}

	//Entity��Component���擾
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

	//Entity��Component���擾
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

	//Entity��Component���J��
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