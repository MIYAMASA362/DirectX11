#include"Common.h"
#include<random>
#include<sstream>

#include"Module\IMGUI\GUI_ImGui.h"

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Object.h"
#include"ObjectManager.h"

//ObjectIndex
//	Object�̃C���X�^���X�ێ�
//
ObjectIndex ObjectManager::g_ObjectIndex;

//DestroyIndex
//	ObjectIndex����폜����^�[�Q�b�g��Index
//
ObjectManager::DestroyIndex ObjectManager::g_DestroyIndex;



//AttachID
//	Object �ɌŗLID��t������
//
InstanceID ObjectManager::AttachID()
{
	std::random_device rand;
	InstanceID id;
	do
	{
		id = rand();
	} 
	while (g_ObjectIndex.find(id) != g_ObjectIndex.end());
	
	return id;
}




//AddIndex
//	ObjectIndex��Object��ǉ�
//
std::weak_ptr<Object> ObjectManager::AddIndex(Object * instance)
{
	return g_ObjectIndex.emplace(instance->GetInstanceID(),std::shared_ptr<Object>(instance)).first->second;
}



//AddDestroy
//	DestroyIndex�֒ǉ�
//
void ObjectManager::AddDestroy(Object * instance)
{
	g_DestroyIndex.push_back(instance->GetInstanceID());
}

//ClearnUp
//	DestroyIndex,ObjectIndex����폜
//
void ObjectManager::ClearnUp()
{
	if (g_DestroyIndex.size() == 0) return;

	for (auto id : g_DestroyIndex)
	{
		auto itr = g_ObjectIndex.find(id);
		itr->second->OnDestroy();
		g_ObjectIndex.erase(itr);
	}
	g_DestroyIndex.clear();
}



//GetInstance
//	ObjectIndex����Object���擾
//
std::weak_ptr<Object> ObjectManager::GetInstance(InstanceID instanceID)
{
	return g_ObjectIndex.at(instanceID);
}

//Release
//	ObjectIndex����ɂ���
//	OnDestroy�֐��̎��s
//
void ObjectManager::Release()
{
	for (auto obj : g_ObjectIndex)
	{
		obj.second->OnDestroy();
	}
	
	g_DestroyIndex.clear();
	g_ObjectIndex.clear();
}

//EditorWindow
//
//
void ObjectManager::EditorWindow()
{
	ImGui::Begin("ObjectManager");
	ImGui::Text("InstanceID[Pointer]");
	ImGui::BeginChild(ImGui::GetID((void*)0),ImVec2(256,512),ImGuiWindowFlags_NoTitleBar);
	for(auto obj :g_ObjectIndex)
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
