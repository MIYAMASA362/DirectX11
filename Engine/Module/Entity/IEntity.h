#pragma once

//IEntity
//	Entity�̊��N���X
//	EntityManager�ɂ���ĊǗ������C���^�[�t�F�C�X
//
class IEntity :public Object
{
	friend class EntityManager;
private:
	//EntityManager�ŊǗ�����Ă���Instance�ւ̃A�N�Z�X
	std::weak_ptr<IEntity> _self;
	//ComponentManager�ŊǗ�����Ă���Components�ւ̃A�N�Z�X
	std::weak_ptr<ComponentList> _components;


public:
	//�R���X�g���N�^
	IEntity();
	//�f�X�g���N�^
	virtual ~IEntity();

	//Entity�̎���ID
	EntityID GetEntityID() { return GetInstanceID(); };
	//Entity�̃|�C���^�擾
	std::weak_ptr<IEntity> GetEntity() { return _self; };

	//Components�ւ̒ǉ�
	template<typename Type> std::weak_ptr<Type> AddComponent();
	//Components����Component�̎擾
	template<typename Type> std::weak_ptr<Type> GetComponent();
	//Components�̎擾
	std::shared_ptr<ComponentList> GetComponents() { return _components.lock(); }
	//Components����Component�̍폜
	template<typename Type> void DestroyComponent();
	//Components�̍폜
	void DestroyComponents();

	//���g�̔j��
	void Destroy() override;


};



//AddComponent
//	Entity��Components�� Type�^��Component��ǉ�
//
template<typename Type> std::weak_ptr<Type> IEntity::AddComponent()
{
	return ComponentManager::AddComponent<Type>(this);
}

//GetComponent
//	Entity��Components�� Type�^��Component���擾
//
template<typename Type> std::weak_ptr<Type> IEntity::GetComponent()
{
	return ComponentManager::GetComponent<Type>(this);
}

//DestroyComponent
//
//
template<typename Type> void IEntity::DestroyComponent()
{
	ComponentManager::DestroyComponent<Type>(this);
}
