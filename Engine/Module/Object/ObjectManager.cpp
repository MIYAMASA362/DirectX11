#include"Common.h"
#include<random>
#include<sstream>
#include<algorithm>

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

}

//~ObjectManager
//	�f�X�g���N�^
//
ObjectManager::~ObjectManager()
{
	_DestroyIndex.clear();

	for (auto object : _ObjectIndex)
	{
		object.second->OnDestroy();
		object.second.reset();
	}
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
//	ObjectIndex�ɒǉ��BObject�ɃX�e�[�^�X�ݒ�
//
//	WARNING : memcpy�Ńf�[�^�u���������Ă��� ���̎����ǂ��Ȃ́H
//
void ObjectManager::RegisterObject(Object * object)
{
	auto sptr = std::shared_ptr<Object>(object);

	std::random_device rand;
	InstanceID id;
	do
	{
		id = rand();
	}
	while (_ObjectIndex.find(id) != _ObjectIndex.end());

	object->_InstanceID = id;
	object->_self = sptr;

	_ObjectIndex.emplace(sptr->_InstanceID,sptr);
}

//DestroyObject
//	DestroyIndex�֒ǉ�
//
void ObjectManager::DestroyObject(Object * object)
{
	_DestroyIndex.push_back(object->GetInstanceID());
}

//ClearnUpObject
//	�I�u�W�F�N�g�폜
//
void ObjectManager::ClearnUpObject()
{
	for(auto id : _DestroyIndex)
	{
		auto find  = _ObjectIndex.find(id);
		if (find == _ObjectIndex.end()) assert(0);
		find->second->OnDestroy();
		_ObjectIndex.erase(find);
	}
	_DestroyIndex.clear();
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
		std::string text = std::to_string(obj.first);
		std::stringstream ss;
		ss << std::hex << (uintptr_t)obj.second.get();
		text += "[" + ss.str()+"]";
		ImGui::Text(text.c_str());
	}
	ImGui::EndChild();
	ImGui::End();
}
