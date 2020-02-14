#include<fstream>

#include"Common.h"
#include"cereal\archives\json.hpp"

#include"Module\DirectX\DirectX.h"

#include"Window\Window.h"
#include"Editor\Editor.h"
#include"Module\Manager\manager.h"

#include"Module\ECSEngine.h"

#include"Module\Tag\Tag.h"
#include"Module\Transform\Transform.h"
#include"Module\GameObject\GameObject.h"

#include"Module\Module.h"

#include"Module\Hierarchy\Hierarchy.h"



#include"Scene.h"
#include"SceneManager.h"



//*********************************************************************************************************************
//
//	Scene
//
//*********************************************************************************************************************

//Scene
//	コンストラクタ
//
Scene::Scene()
{

}

//Scene
//	コンストラクタ
//
Scene::Scene(std::string name, std::string path)
	:
	_name(name),
	_filePath(path)
{
	
}

//~Scene
//	デストラクタ
//
inline Scene::~Scene()
{

}




bool Scene::CompareName(std::string name)
{
	return this->_name == name;
}

//AddSceneObject
//	Sceneにオブジェクトを追加する
//
std::shared_ptr<GameObject> Scene::AddSceneObject(std::string name, TagName tag)
{
	//GameObject
	auto gameObject = CreateSceneObject(name,tag);

	//階層追加
	_hierarchyUtility.AttachHierarchy(gameObject);

	//Transform
	gameObject->AddComponent<Transform>();

	return gameObject;
}

//CreateSceneObject
//	GameObjectを生成する
//
std::shared_ptr<GameObject> Scene::CreateSceneObject(std::string name, TagName tag)
{
	auto result = std::shared_ptr<GameObject>(new GameObject(name, this, tag));

	ObjectManager::GetInstance()->RegisterObject(result);
	EntityManager::GetInstance()->RegisterEntity(result);
	GameObject::RegisterEntityIndex(result);

	return result;
}

//RemoveSceneObject
//	Sceneからオブジェクト削除
//
void Scene::RemoveSceneObject(GameObject * gameobject)
{
	_hierarchyUtility.DetachHierarchy(gameobject->GetEntityID());
	gameobject->Destroy();
}

//AttachActiveScene
//	アクティブなシーンにする
//
void Scene::AttachActiveScene()
{
	this->IsActive = true;
	//Scene読み込み
	this->Load();
}

//DetachActiveScene
//	非アクティブなシーンにする
//
void Scene::DetachActiveScene()
{
	this->IsActive = false;
	//Scene破棄
	this->UnLoad();
}

static GameObject* Debug_InspectorObject = nullptr;

//DebugGUI_SceneHierarchy
//	HierarchyのDebug表示
//
void DebugGUI_SceneHierarchy(std::list<std::weak_ptr<IEntity>> children, HierarchyUtility* utility)
{
	for (auto child : children)
	{
		auto instance = EntityManager::GetInstance()->GetEntity(child.lock()->GetEntityID()).lock();
		auto gameObject = std::dynamic_pointer_cast<GameObject>(instance);
		if (ImGui::TreeNode(gameObject->GetName().c_str()))
		{
			if (ImGui::Button("Inspector"))
			{
				Debug_InspectorObject = gameObject.get();
			}
			DebugGUI_SceneHierarchy(utility->GetChildren(child.lock()->GetEntityID()), utility);
			ImGui::TreePop();
		}
	}
}

//DebugGUI
//	デバッグ表示
//
void Scene::DebugGUI()
{
	for (auto obj : _hierarchyUtility.GetHierarchyMap())
	{
		if (!obj.second.GetParent().expired()) continue;
		auto instance = EntityManager::GetInstance()->GetEntity(obj.first).lock();
		auto gameObject = std::dynamic_pointer_cast<GameObject>(instance);
		if (ImGui::TreeNode("　"))
		{
			DebugGUI_SceneHierarchy(obj.second.GetChildren(), &_hierarchyUtility);
			ImGui::TreePop();
		}
		ImGui::SameLine();
		if (ImGui::Button(gameObject->GetName().data()))
		{
			CManager::Editor().CreateInspector(gameObject);

			Debug_InspectorObject = gameObject.get();
		}
	}

	if (Debug_InspectorObject != nullptr)
	{
		ImGui::Begin("Inspector");
		Debug_InspectorObject->OnDebugGUI();
		ImGui::End();
	}
	

}

//Load
//	デシリアライズ
//
void Scene::Load()
{
	//デシリアライズ
	std::ifstream file(_filePath);
	cereal::JSONInputArchive inArchive(file);
	inArchive(*this);

	//取得したデータから再構成
	for (auto map : this->_hierarchyUtility.GetHierarchyMap())
	{
		Hierarchy& hierarchy = map.second;
		auto gameObject = std::static_pointer_cast<GameObject>(hierarchy.GetSelf().lock());
		gameObject->_Scene = this;
		auto index = ObjectManager::GetInstance()->_ObjectIndex;

		ObjectManager::GetInstance()->RegisterObject(gameObject);
		EntityManager::GetInstance()->RegisterEntity(gameObject);
		GameObject::RegisterEntityIndex(gameObject);
	}
}

//Save
//	シリアライズ
//
void Scene::Save()
{
	std::ofstream file(_filePath);
	cereal::JSONOutputArchive output(file);
	output(*this);
}

void Scene::ReleaseObjects()
{
	this->UnLoad();
}

//UnLoad
//	
//
void Scene::UnLoad()
{
	for (auto obj : _hierarchyUtility.GetHierarchyMap())
	{
		RemoveSceneObject((GameObject*)obj.second.GetSelf().lock().get());
		//std::dynamic_pointer_cast<GameObject>(EntityManager::GetInstance()->GetEntity(obj.first).lock())->Destroy();
	}
	_hierarchyUtility.ClearnHierarchy();
}