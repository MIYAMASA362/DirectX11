#pragma once

//*********************************************************************************************************************
//
//	ObjectManager
//		Object�̊Ǘ�
//
//*********************************************************************************************************************
class ObjectManager final
{
private:
	//�C���X�^���X
	static ObjectManager* pInstance;


	//�R���X�g���N�^
	ObjectManager();
	//�f�X�g���N�^
	~ObjectManager();

public:
	//Object Instance�C���f�b�N�X
	std::unordered_map<InstanceID, std::shared_ptr<Object>> _ObjectIndex;

	//Object Instance�@�폜�C���f�b�N�X
	std::vector<InstanceID> _DestroyIndex;
	//�C���X�^���X�̎擾
	static ObjectManager* GetInstance() { return pInstance; }

	//�C���X�^���X����
	static void Create();
	//�C���X�^���X�j��
	static void Destroy();

	//Object�o�^
	std::shared_ptr<Object> RegisterObject(std::shared_ptr<Object> object);
	//Obejct�̔j��
	void ReleaseObject(Object* object);
	//�o�^Object�̎擾
	std::shared_ptr<Object> GetObjectInstance(InstanceID id);

	//Object�폜�o�^
	void DestroyObject(Object* object);
	//Object�폜 Object::OnDestory()�̎��s
	void ClearnUpObject();

	//Editor�f�o�b�O�\��
	void EditorWindow();
};