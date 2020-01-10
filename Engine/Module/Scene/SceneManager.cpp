#include"Common.h"

//DirectX
#include"Module\DirectX\DirectX.h"

//ECS
#include"Module\ECSEngine.h"

//Component

#include"Module\Tag\Tag.h"

#include"Module\Transform\Transform.h"
#include"Module\GameObject\GameObject.h"
#include"Module\Renderer\Renderer.h"
#include"Module\Camera\camera.h"
#include"Module\Physics\Rigidbody.h"

#include"Module\UI\UI.h"

#include"SceneManager.h"
#include"Module\Physics\Collision.h"

#include"Module\Hierarchy\Hierarchy.h"

#include"../IMGUI/GUI_ImGui.h"

using namespace DirectX;

//*********************************************************************************************************************
//
//	SceneManager
//
//*********************************************************************************************************************

//SceneIDのカウント
SceneID SceneManager::m_SceneID;
//Sceneの保管庫
std::map<SceneID, std::shared_ptr<Scene>> SceneManager::pSceneDictionary;
//現在のScene
std::weak_ptr<Scene> SceneManager::pActiveScene;
//次のScene
std::weak_ptr<Scene> SceneManager::pNextScene;
//遷移が有効か
bool SceneManager::IsChangeScene = false;

//Destroy
//	Scene削除
//
void SceneManager::Destroy()
{
	if (!pActiveScene.expired())
		pActiveScene.reset();
	if (!pNextScene.expired()) 
		pNextScene.reset();

	pSceneDictionary.clear();
}

//Scene読み込み
void SceneManager::LoadScene(std::weak_ptr<Scene> scene)
{
	//ActiveSceneが無い
	if(pActiveScene.expired())
		return AttachActiveScene(scene);

	//次のSceneへ登録
	SetIsChangeScene(scene);
}

//ActiveScene内オブジェクトのImGUI表示
void SceneManager::DebugGUI_ActiveScene()
{
	std::string label = "Scene:";
	label += pActiveScene.lock()->GetSceneName();
	ImGui::Begin("Scene");
	ImGui::Text(label.c_str());
	pActiveScene.lock()->DebugGUI();
	ImGui::End();
}

//Scene遷移
void SceneManager::ChangeScene()
{
	if (!IsChangeScene) return;	//遷移フラグが有効
	//無効化
	pNextScene = pNextScene;
	DetachActiveScene();
	//有効化
	pNextScene = pNextScene;
	AttachActiveScene(pNextScene.lock());

	ObjectManager::ClearnUp();
	
	ComponentManager::SendComponentMessage("Start");

	IsChangeScene = false;
}

//ActiveSceneの取得
std::weak_ptr<Scene> SceneManager::GetActiveScene()
{
	return pActiveScene;
}

std::weak_ptr<Scene> SceneManager::GetScene(SceneID id)
{
	return pSceneDictionary.at(id);
}

//Scene名からSceneを取得
std::weak_ptr<Scene> SceneManager::GetSceneByName(std::string SceneName)
{
	for (auto scene : pSceneDictionary) {
		if (scene.second->CompareName(SceneName))
			return scene.second;
	}
	return std::shared_ptr<Scene>(nullptr);
}

//次のSceneを設定
void SceneManager::SetIsChangeScene(std::weak_ptr<Scene> scene)
{
	pNextScene = scene;
	IsChangeScene = true;
}

//ActiveSceneを設定
void SceneManager::AttachActiveScene(std::weak_ptr<Scene> scene)
{
	pActiveScene = scene;
	pActiveScene.lock()->AttachActiveScene();
}

//ActiveSceneを破棄
void SceneManager::DetachActiveScene()
{
	pActiveScene.lock()->DetachActiveScene();
	pActiveScene.reset();
}

SceneID SceneManager::AttachID()
{
	SceneID id = m_SceneID;
	m_SceneID++;
	return id;
}




//*********************************************************************************************************************
//
//	Scene
//
//*********************************************************************************************************************

//Scene
//	コンストラクタ
//
Scene::Scene(std::string name)
	:
	m_id(SceneManager::AttachID()),
	m_name(name),
	_hierarchyUtility(new HierarchyUtility())
{

};

//~Scene
//	デストラクタ
//
inline Scene::~Scene()
{
	delete _hierarchyUtility;
};

//AddSceneObject
//	Sceneにオブジェクトを追加する
//
GameObject* Scene::AddSceneObject(std::string name,TagName tag)
{
	auto instance = new GameObject(name, this, tag);

	//階層追加
	_hierarchyUtility->AttachHierarchy(instance->GetEntityID());

	//Transform
	instance->AddComponent<Transform>();
	return instance;
}

//RemoveSceneObject
//	Sceneからオブジェクト削除
//
void Scene::RemoveSceneObject(EntityID id)
{
	_hierarchyUtility->DetachHierarchy(id);
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
void DebugGUI_SceneHierarchy(std::list<std::weak_ptr<IEntity>> children,HierarchyUtility* utility)
{
	for(auto child : children)
	{
		auto gameObject = std::dynamic_pointer_cast<GameObject>(EntityManager::GetEntity(child.lock()->GetEntityID()).lock());
		if(ImGui::TreeNode(gameObject->GetName().c_str()))
		{
			if (ImGui::Button("Inspector"))
			{
				Debug_InspectorObject = gameObject.get();
			}
			DebugGUI_SceneHierarchy(utility->GetChildren(child.lock()->GetEntityID()),utility);
			ImGui::TreePop();
		}
	}
}

//DebugGUI
//	デバッグ表示
//
void Scene::DebugGUI()
{
	for(auto obj : _hierarchyUtility->GetHierarchyMap())
	{
		if (!obj.second.GetParent().expired()) continue;
		auto gameObject = std::dynamic_pointer_cast<GameObject>(EntityManager::GetEntity(obj.first).lock());
		if(ImGui::TreeNode(gameObject->name.c_str()))
		{
			if (ImGui::Button("Inspector"))
			{
				Debug_InspectorObject = gameObject.get();
			}
			DebugGUI_SceneHierarchy(obj.second.GetChildren(),_hierarchyUtility);
			ImGui::TreePop();
		}
	}

	if(Debug_InspectorObject != nullptr)
	{
		ImGui::Begin("Inspector");
		Debug_InspectorObject->OnDebugGUI();
		ImGui::End();
	}
}

//UnLoad
//	
//
void Scene::UnLoad()
{
	for (auto obj : _hierarchyUtility->GetHierarchyMap())
		std::dynamic_pointer_cast<GameObject>(EntityManager::GetEntity(obj.first).lock())->Destroy();
	_hierarchyUtility->ClearnHierarchy();
}