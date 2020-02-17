#include"Common.h"
#include<random>
#include<sstream>
#include<algorithm>

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\IMGUI\GUI_ImGui.h"

#include"Object.h"
#include"ObjectManager.h"


//*********************************************************************************************************************
//
//	ObjectManager
//
//*********************************************************************************************************************

//�C���X�^���X
ObjectManager* ObjectManager::pInstance = nullptr;



//ObjectManager
//	�R���X�g���N�^
//
ObjectManager::ObjectManager()
{
	_ObjectIndex.reserve(OBJECT_CAPACITY);
}

//~ObjectManager
//	�f�X�g���N�^
//
ObjectManager::~ObjectManager()
{
	//�I�u�W�F�N�g�z����N���A
	_ObjectIndex.clear();
}

//Create
//	�C���X�^���X����
//
void ObjectManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new ObjectManager();
}

//Destroy
//	�C���X�^���X�j��
//
void ObjectManager::Destroy()
{
	if (pInstance == nullptr) return;
	delete pInstance;
	pInstance = nullptr;
}

//RegisterObject
//	ObjectIndex�ɒǉ�
//
void ObjectManager::RegisterObject(std::shared_ptr<Object> instance)
{
	_ObjectIndex.push_back(instance);
	instance->Register(instance);
}

//DestroyObject
//	DestroyIndex�֒ǉ�
//
void ObjectManager::DestroyObject(Object * object)
{
	_DestroyIndex.push(object->GetInstanceID());
}

//ClearnUpObject
//	�I�u�W�F�N�g�폜
//
void ObjectManager::ClearnUpObject()
{
	if (_DestroyIndex.size() == 0) return;

	InstanceID id;
	std::vector<std::shared_ptr<Object>>::iterator itr;

	//�폜�Ώۂ��ړ�
	unsigned int i = 0;
	for(size_t size = _DestroyIndex.size(); i < size; i++)
	{
		id = _DestroyIndex.top();

		itr = std::find_if(
			_ObjectIndex.begin(), _ObjectIndex.end(), [id](std::shared_ptr<Object> object) 
		{
			return object->GetInstanceID() == id;
		});

		//�폜�����s�֐�
		(*itr)->Release();

		//���S�폜
		_ObjectIndex.erase(itr);

		_DestroyIndex.pop();
	}
}


//EditorWindow
//
//
void ObjectManager::EditorWindow()
{
	ImGui::Begin("ObjectManager");
	ImGui::Text("InstanceID[Pointer]");
	ImGui::BeginChild(ImGui::GetID((void*)0),ImVec2(256,512),ImGuiWindowFlags_NoTitleBar);
	for(auto obj :_ObjectIndex)
	{
		std::string text = std::to_string(obj->GetInstanceID());
		std::stringstream ss;
		ss << std::hex << (uintptr_t)obj.get();
		text += "[" + ss.str()+"]";
		ImGui::Text(text.c_str());
	}
	ImGui::EndChild();
	ImGui::End();
}
