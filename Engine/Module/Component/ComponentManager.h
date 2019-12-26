#pragma once

//Entity�ɕt������Components
using EntityComponents = std::unordered_map<EntityID, std::shared_ptr<ComponentList>>;

class IEntity;

//ComponentManager
//	Entity��Component�Ƃ̊ւ����Ǘ�
//
class ComponentManager final
{
private:
	//�C���X�^���X
	static ComponentManager* g_pInstance;

	//Entity�ɕt�����ꂽComponents�̃C���f�b�N�X
	EntityComponents _EntityComponentIndex;


private:
	//�R���X�g���N�^
	ComponentManager();
	//�f�X�g���N�^
	~ComponentManager();


public:
	//�C���X�^���X����
	static void Create();
	//�C���X�^���X�j��
	static void Release();


	//Component�ɑ΂���message���M
	static void SendComponentMessage(std::string message);
	//�����Entity��Component��message���M
	static void SendComponentMessage(std::string message, EntityID entityID);


	//Entity��Component���X�g�𐶐�
	static std::weak_ptr<ComponentList> CreateComponents(IEntity* entity);
	//Entity��Component���X�g���擾
	static std::weak_ptr<ComponentList> GetComponents(IEntity* entity);
	//Entity��Components��ObjectManager�̊��S�폜
	static void DestroyComponents(IEntity* entity);
	//Entity��Components�����
	static void ReleaseComponents(IEntity* entity);


	//Entity��Component��ǉ�
	template<typename Type>  static std::shared_ptr<Type> AddComponent(IEntity* entity);
	//Entity��Component���擾
	template<typename Type>  static std::shared_ptr<Type> GetComponent(IEntity* entity);
	//Entity��Component�����
	template<typename Type> static void ReleaseComponent(IEntity* entity);

	//�SComponent��Debug�\��
	static void ImGui_ComponentView(EntityID id);


};



//Entity��Component��ǉ�
template<typename Type>
inline std::shared_ptr<Type> ComponentManager::AddComponent(IEntity* entity)
{
	//�C���X�^���X����
	Object* instance = new Type(entity->GetEntityID());
	std::shared_ptr<Type> component = std::dynamic_pointer_cast<Type>(ObjectManager::GetInstance(instance->GetInstanceID()).lock());
	entity->GetComponents()->Add(component);
	return component;
}

//Entity��Component���擾
template<typename Type >
inline std::shared_ptr<Type> ComponentManager::GetComponent(IEntity* entity)
{
	return std::dynamic_pointer_cast<Type>(entity->GetComponents()->Get(Component<Type>::GetTypeID()).lock());
}

//Entity��Component���J��
template<typename Type>
inline void ComponentManager::ReleaseComponent(IEntity* entity)
{
	entity->GetComponents()->Remove(Component<Type>::GetTypeID());
}