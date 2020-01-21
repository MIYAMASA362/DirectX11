#pragma once

//Entity�ɕt������Components
using EntityComponents = std::unordered_map<EntityID, std::shared_ptr<ComponentList>>;

class IEntity;

//*********************************************************************************************************************
//
//	ComponentManager
//
//*********************************************************************************************************************
class ComponentManager final
{
private:
	//�C���X�^���X
	static ComponentManager* pInstance;

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

	static ComponentManager* GetInstance() { return pInstance; };

	//Component�ɑ΂���message���M
	void SendComponentMessage(std::string message);
	//�����Entity��Component��message���M
	void SendComponentMessage(std::string message, EntityID entityID);


	//Entity��Component���X�g�𐶐�
	std::weak_ptr<ComponentList> CreateComponents(IEntity* entity);
	//Entity��Component���X�g���擾
	std::weak_ptr<ComponentList> GetComponents(IEntity* entity);
	//Entity��Components��ObjectManager�̊��S�폜
	void DestroyComponents(IEntity* entity);
	//Entity��Components�����
	void ReleaseComponents(IEntity* entity);


	//Entity��Component��ǉ�
	template<typename Type> 
	std::shared_ptr<Type> AddComponent(IEntity* entity);
	//Entity��Component���擾
	template<typename Type>
	std::shared_ptr<Type> GetComponent(IEntity* entity);
	//Entity��Component�����
	template<typename Type>
	void ReleaseComponent(IEntity* entity);

	//�SComponent��Debug�\��
	void ImGui_ComponentView(EntityID id);


};



//Entity��Component��ǉ�
template<typename Type>
inline std::shared_ptr<Type> ComponentManager::AddComponent(IEntity* entity)
{
	//�C���X�^���X����
	auto* instance = new Type(entity->GetEntityID());
	ObjectManager::GetInstance()->RegisterObject(instance);
	Component<Type>::RegisterComponentIndex(instance);

	std::shared_ptr<Type> component = std::dynamic_pointer_cast<Type>(instance->GetSelf().lock());

	entity->GetComponents()->Add(component);
	return component;
}

//Entity��Component���擾
template<typename Type >
inline std::shared_ptr<Type> ComponentManager::GetComponent(IEntity* entity)
{
	return std::dynamic_pointer_cast<Type>(entity->GetComponents()->Get(typeid(Type).hash_code()).lock());
}

//Entity��Component���J��
template<typename Type>
inline void ComponentManager::ReleaseComponent(IEntity* entity)
{
	entity->GetComponents()->Remove(Component<Type>::GetTypeID());
}