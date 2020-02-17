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

//インスタンス
ObjectManager* ObjectManager::pInstance = nullptr;



//ObjectManager
//	コンストラクタ
//
ObjectManager::ObjectManager()
{
	_ObjectIndex.reserve(OBJECT_CAPACITY);
}

//~ObjectManager
//	デストラクタ
//
ObjectManager::~ObjectManager()
{
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
//	ObjectIndexに追加
//
void ObjectManager::RegisterObject(std::shared_ptr<Object> instance)
{
	_ObjectIndex.push_back(instance);
	instance->Register(instance);
}

//DestroyObject
//	DestroyIndexへ追加
//
void ObjectManager::DestroyObject(Object * object)
{
	_DestroyIndex.push(object->GetInstanceID());
}

//ClearnUpObject
//	オブジェクト削除
//
void ObjectManager::ClearnUpObject()
{
	if (_DestroyIndex.size() == 0) return;

	InstanceID id;
	std::vector<std::shared_ptr<Object>>::iterator itr;

	//削除対象を移動
	unsigned int i = 0;
	for(size_t size = _DestroyIndex.size(); i < size; i++)
	{
		id = _DestroyIndex.top();

		itr = std::find_if(
			_ObjectIndex.begin(), _ObjectIndex.end(), [id](std::shared_ptr<Object> object) 
		{
			return object->GetInstanceID() == id;
		});

		//削除時実行関数
		(*itr)->Release();

		//完全削除
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
