#pragma once

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
	std::unordered_map<EntityID, std::shared_ptr<ComponentList>> _EntityComponentIndex;


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

	void AddComponentInstance(IEntity* owner, IComponent* original);
};


//AddComponent
//
//	ObjectManager�ɃC���X�^���X��o�^
//	Component<Type>�ɃC���X�^���X��o�^
//
//	->�Ǘ������f�[�^�̐݌v������
//
template<typename Type>
inline std::shared_ptr<Type> ComponentManager::AddComponent(IEntity* entity)
{
	//�C���X�^���X����
	IComponent* instance = new Type(entity->GetEntityID());

	std::shared_ptr<Type> component = std::dynamic_pointer_cast<Type>(instance->GetSelf());

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