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
	//Entity ComponentList
	static std::unordered_map<EntityID, std::shared_ptr<ComponentList>> _EntityComponentIndex;

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

	//�C���X�^���X
	static ComponentManager* GetInstance() { return pInstance; };

	//ComponentList

	//Entity��ComponentList�𐶐�
	std::weak_ptr<ComponentList> CreateComponents(IEntity* entity);
	//Entity��ComponentList�̍폜����
	void DestroyComponents(IEntity* entity);
	//Entity��ComponentList�̔j��
	void ReleaseComponents(IEntity* entity);
	//Entity��ComponentList��u������
	std::weak_ptr<ComponentList> SwapComponents(IEntity* entity,ComponentList* ComponentList);
	std::weak_ptr<ComponentList> SwapComponents(IEntity* entity,std::shared_ptr<ComponentList> ComponentList);

	//Component

	//Entity��Component��ǉ�
	template<typename Type> 
	std::shared_ptr<Type> AddComponent(IEntity* entity);
	//Entity��Component���擾
	template<typename Type>
	std::shared_ptr<Type> GetComponent(IEntity* entity);

	//Component�ɑ΂���message���M
	void SendComponentMessage(std::string message);
	//�����Entity��Component��message���M
	void SendComponentMessage(std::string message, EntityID entityID);

	//�SComponent��Debug�\��
	void ImGui_ComponentView(EntityID id);
};


//AddComponent
//	Component�̃C���X�^���X����
//	ObjectManager�֒ǉ�
//
template<typename Type>
std::shared_ptr<Type> ComponentManager::AddComponent(IEntity* entity)
{
	auto component = std::shared_ptr<Type>(new Type(entity->GetEntityID()));
	//ObjectManager�֒ǉ�
	ObjectManager::GetInstance()->RegisterObject(component);
	Component<Type>::RegisterComponentIndex(component);
	//Components�֒ǉ�
	entity->GetComponents()->AddComponent(component);

	return component;
}

//Entity��Component���擾
template<typename Type >
std::shared_ptr<Type> ComponentManager::GetComponent(IEntity* entity)
{
	return std::dynamic_pointer_cast<Type>(entity->GetComponents()->GetComponent((ComponentTypeID)typeid(Type).hash_code()));
}