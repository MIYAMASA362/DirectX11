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
	//Component
	std::vector<std::weak_ptr<IComponent>> _ComponentIndex;


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


	//Component��Message���M
	void SendComponentMessage(std::string message);

	//ComponentIndex�֒ǉ�
	void RegisterComponent(std::weak_ptr<IComponent> component);
	//ComponentIndex����폜
	void ReleaseComponent(IComponent* component);
};