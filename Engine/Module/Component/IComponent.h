#pragma once

class Transform;
class GameObject;



//IComponent
//	ComponentManager�ŊǗ������Component Intarface
//
class IComponent:public Object
{
	//Component�̊Ǘ�
	friend class ComponentManager;
private:
	//OwnerID ���LEntity��ID
	const EntityID _ownerId;
	//Entity GameObject
	std::weak_ptr<GameObject> _gameObject;


protected:
	//ComponentManager�ŊǗ�����Ă���Component
	std::weak_ptr<IComponent> _self;

	//Component�Ƀ��b�Z�[�W�𑗐M���ꂽ���̓���
	std::function<void(std::string)> SendComponentMessage = {};
	//ImGui�̐ݒ�
	std::function<void(void)> OnDebugImGui = {};


public:
	//�R���X�g���N�^
	IComponent(EntityID OwnerID);
	//�f�X�g���N�^
	virtual ~IComponent();

	//OwnerID�̎擾
	const EntityID GetOwnerID() const { return _ownerId; };
	//ComponentTypeID�̎擾
	virtual const ComponentTypeID GetComponentTypeID () const =0;
	//ComponentID�̎擾
	ComponentID GetComponentID() { return GetInstanceID(); };

	//Entity��Tranform�ւ̃A�N�Z�X
	std::shared_ptr<Transform> transform();
	//Entity��GameObject�ւ̃A�N�Z�X
	std::shared_ptr<GameObject> gameObject() { return _gameObject.lock(); };


protected:
	//�폜�����s�֐�
	virtual void OnDestroy() override = 0;


};