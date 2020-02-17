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
	std::vector<std::shared_ptr<Object>> _ObjectIndex;
	//Object Instance�@�폜�C���f�b�N�X
	std::stack<InstanceID> _DestroyIndex;


	//�C���X�^���X�̎擾
	static ObjectManager* GetInstance() { return pInstance; }

	//�C���X�^���X����
	static void Create();
	//�C���X�^���X�j��
	static void Destroy();

	//Object�o�^
	void RegisterObject(std::shared_ptr<Object> instance);

	//Object�폜�o�^
	void DestroyObject(Object* object);
	//Object�폜 Object::OnDestory()�̎��s
	void ClearnUpObject();

	//Editor�f�o�b�O�\��
	void EditorWindow();


};