#include<fstream>

#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include"Module\ECSEngine.h"

#include"Module\Tag\Tag.h"
#include"Module\Transform\Transform.h"
#include"Module\GameObject\GameObject.h"

#include"Module\Hierarchy\Hierarchy.h"

#include"Scene.h"
#include"SceneManager.h"

#include"cereal\archives\json.hpp"


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
GameObject* Scene::AddSceneObject(std::string name, TagName tag)
{
	auto instance = new GameObject(name, this, tag);

	ObjectManager::GetInstance()->RegisterObject(instance);
	EntityManager::GetInstance()->RegisterEntity(instance);
	GameObject::RegisterEntityIndex(instance);

	//階層追加
	_hierarchyUtility.AttachHierarchy(instance->GetEntityID());

	//Transform
	instance->AddComponent<Transform>();
	auto transform = instance->transform();
	transform = transform;

	return instance;
}

//RemoveSceneObject
//	Sceneからオブジェクト削除
//
void Scene::RemoveSceneObject(GameObject * gameobject)
{
	_hierarchyUtility.DetachHierarchy(gameobject->GetEntityID());

	GameObject::DestroyEntityIndex(gameobject);
	EntityManager::GetInstance()->DestroyEntity(gameobject);
	ObjectManager::GetInstance()->DestroyObject(gameobject);
}

//AttachActiveScene
//	アクティブなシーンにする
//
void Scene::AttachActiveScene()
{
	this->IsLoaded = true;
	this->Load();	//読み込み
}

//DetachActiveScene
//	非アクティブなシーンにする
//
void Scene::DetachActiveScene()
{
	this->IsLoaded = false;
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
		if (ImGui::TreeNode(gameObject->_Name.c_str()))
		{
			if (ImGui::Button("Inspector"))
			{
				Debug_InspectorObject = gameObject.get();
			}
			DebugGUI_SceneHierarchy(obj.second.GetChildren(), &_hierarchyUtility);
			ImGui::TreePop();
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
	std::ifstream file(_filePath);
	cereal::JSONInputArchive inArchive(file);
	inArchive(*this);
}

//Save
//	シリアライズ
//
void Scene::Save()
{
	/*std::ofstream file(_filePath);
	cereal::JSONOutputArchive output(file);
	output(*this);*/
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