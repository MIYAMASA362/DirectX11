#pragma once

//*********************************************************************************************************************
//
//	EntityManager
//
//*********************************************************************************************************************
class EntityManager final
{
private:
	//�C���X�^���X
	static EntityManager* pInstacne;

	//�SEntity�̃C���f�b�N�X
	std::vector<std::weak_ptr<IEntity>> _EntityIndex;

	//�R���X�g���N�^
	EntityManager();
	//�f�X�g���N�^
	~EntityManager();


public:
	//EntityManager�̃C���X�^���X����
	static void Create();
	//EntityManager�̃C���X�^���X����
	static void Release();
	//�C���X�^���X�̎擾
	static EntityManager* GetInstance() { return pInstacne; };

	//EntityIndex�֒ǉ�
	void RegisterEntity(std::shared_ptr<IEntity> instance);
	//EntityIndex����Entity�̍폜
	void ReleaseEntity(IEntity * instance);


};