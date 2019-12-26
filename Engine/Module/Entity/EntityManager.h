#pragma once

//EntityIndex
using EntityIndex = std::unordered_map<EntityID, std::weak_ptr<IEntity>>;

//EntityManager
//	IEntity�̊Ǘ�
//
class EntityManager final
{
private:
	//�C���X�^���X
	static EntityManager* g_pInstacne;

	//�SEntity�̃C���f�b�N�X
	EntityIndex* _EntityIndex;

	//�R���X�g���N�^
	EntityManager();
	//�f�X�g���N�^
	~EntityManager();


public:
	//EntityManager�̃C���X�^���X����
	static void Create();
	//EntityManager�̃C���X�^���X����
	static void Release();

	//EntityIndex�֒ǉ�
	static std::weak_ptr<IEntity> CreateEntity(IEntity* instance);
	//EntityIndex����Entity�̍폜
	static void ReleaseEntity(IEntity * instance);

	//EntityIndex����Entity�̎擾
	static std::weak_ptr<IEntity> GetEntity(EntityID id);


};