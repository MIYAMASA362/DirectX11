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

//インスタンス
ObjectManager* ObjectManager::pInstance = nullptr;



//ObjectManager
//	コンストラクタ
//
ObjectManager::ObjectManager()
{
	
}

//~ObjectManager
//	デストラクタ
//
ObjectManager::~ObjectManager()
{
	//削除配列をクリア
	_DestroyIndex.clear();

	//オブジェクト配列からオブジェクト削除
	for (auto object : _ObjectIndex)
	{
		object.second.reset();
	}

	//オブジェクト配列をクリア
	_ObjectIndex.clear();
}

//Create
//	インスタンス生成
//
void ObjectManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new ObjectManager();
}

//Destroy
//	インスタンス破棄
//
void ObjectManager::Destroy()
{
	if (pInstance == nullptr) return;
	delete pInstance;
	pInstance = nullptr;
}

//RegisterObject
//	ObjectIndexに追加。Objectにステータス設定
//
std::shared_ptr<Object> ObjectManager::RegisterObject(std::shared_ptr<Object> object)
{
	return _ObjectIndex.emplace(object->GetInstanceID(),object).first->second;
}

std::shared_ptr<Object> ObjectManager::GetObjectInstance(InstanceID id)
{
	auto find = _ObjectIndex.find(id);
	if (find == _ObjectIndex.end()) assert(0);
	return find->second;
}

void ObjectManager::ReleaseObject(Object* object)
{
	object->Release();
	_ObjectIndex.erase(object->GetInstanceID());
}

//DestroyObject
//	DestroyIndexへ追加
//
void ObjectManager::DestroyObject(Object * object)
{
	_DestroyIndex.push_back(object->GetInstanceID());
}

//ClearnUpObject
//	オブジェクト削除
//
void ObjectManager::ClearnUpObject()
{
	for(auto id : _DestroyIndex)
	{
		auto find  = _ObjectIndex.find(id);
		if (find == _ObjectIndex.end()) assert(0);
		this->ReleaseObject(find->second.get());
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
