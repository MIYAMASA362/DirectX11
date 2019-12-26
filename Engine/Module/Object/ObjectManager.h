#pragma once

//�S�I�u�W�F�N�g
using ObjectIndex = std::unordered_map<InstanceID, std::shared_ptr<Object>>;

//ObjectManager
//	�Q�[������Object���Ǘ�����
//
class ObjectManager
{
private:
	//Object Instance�C���f�b�N�X
	static ObjectIndex g_ObjectIndex;

	//Object�̍폜�p�C���f�b�N�X
	using DestroyIndex = std::vector<InstanceID>;
	//Object Instance�@�폜�C���f�b�N�X
	static DestroyIndex g_DestroyIndex;


public:
	//Object �ɌŗLID��t������
	static InstanceID AttachID();

	//g_ObjectIndex�ɒǉ�����
	static std::weak_ptr<Object> AddIndex(Object* instance);

	//g_DestroyIndex�ɒǉ�����
	static void AddDestroy(Object* instance);
	//g_DestroyIndex���̃I�u�W�F�N�g���폜����
	static void ClearnUp();

	//g_ObjectIndex����Index�̎擾
	static std::weak_ptr<Object> GetInstance(InstanceID instanceID);
	
	//g_ObjectIndex���폜
	static void Release();



};