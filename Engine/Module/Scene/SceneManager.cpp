#include"Common.h"

//DirectX
#include"Module\DirectX\DirectXStruct.h"
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

#include"Module\SystemManager\SystemManager.h"
#include"SceneManager.h"
#include"Module\Physics\Collision.h"

#include"../IMGUI/GUI_ImGui.h"

using namespace DirectX;


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

//インスタンス削除
void SceneManager::Destroy()
{
	if (!pActiveScene.expired())
		pActiveScene.reset();
	if (!pNextScene.expired())
		pNextScene.reset();

	pSceneDictionary.clear();
}

//Scene読み込み
void DirectX::SceneManager::LoadScene(std::weak_ptr<Scene> scene)
{
	//ActiveSceneが無い
	if(pActiveScene.expired())
		return AttachActiveScene(scene);

	//次のSceneへ登録
	SetIsChangeScene(scene);
}

//ActiveScene内オブジェクトのImGUI表示
void DirectX::SceneManager::DebugGUI_ActiveScene()
{
	std::string label = "Scene:";
	label += pActiveScene.lock()->GetSceneName();
	ImGui::Begin("Scene");
	ImGui::Text(label.c_str());
	pActiveScene.lock()->DebugGUI();
	ImGui::End();
}

//Scene遷移
void DirectX::SceneManager::ChangeScene()
{
	
	if (!IsChangeScene) return;	//遷移フラグが有効
	//無効化
	pNextScene = pNextScene;
	DetachActiveScene();
	//有効化
	pNextScene = pNextScene;
	AttachActiveScene(pNextScene.lock());
	IsChangeScene = false;
}

//ActiveSceneの取得
std::weak_ptr<Scene> DirectX::SceneManager::GetActiveScene()
{
	return pActiveScene;
}

std::weak_ptr<Scene> DirectX::SceneManager::GetScene(SceneID id)
{
	return pSceneDictionary.at(id);
}

//Scene名からSceneを取得
std::weak_ptr<Scene> DirectX::SceneManager::GetSceneByName(std::string SceneName)
{
	for (auto scene : pSceneDictionary) {
		if (scene.second->CompareName(SceneName))
			return scene.second;
	}
	return std::shared_ptr<Scene>(nullptr);
}

//次のSceneを設定
void DirectX::SceneManager::SetIsChangeScene(std::weak_ptr<Scene> scene)
{
	pNextScene = scene;
	IsChangeScene = true;
}

//ActiveSceneを設定
void DirectX::SceneManager::AttachActiveScene(std::weak_ptr<Scene> scene)
{
	pActiveScene = scene;
	pActiveScene.lock()->AttachActiveScene();
}

//ActiveSceneを破棄
void DirectX::SceneManager::DetachActiveScene()
{
	pActiveScene.lock()->DetachActiveScene();
	pActiveScene.reset();
}

SceneID DirectX::SceneManager::AttachID()
{
	SceneID id = m_SceneID;
	m_SceneID++;
	return id;
}

//GameObjectをTag指定で追加
GameObject* DirectX::Scene::AddSceneObject(std::string name,TagName tag)
{
	auto instance = new GameObject(name, this, tag);
	this->Index.push_back(instance->GetEntityID());
	instance->AddComponent<Transform>();
	return instance;
}

//
void DirectX::Scene::AttachActiveScene()
{
	this->IsLoaded = true;
	this->Load();	//読み込み
}

//
void DirectX::Scene::DetachActiveScene()
{
	this->IsLoaded = false;
	this->UnLoad();
}

//ImGui Debug表示
void DirectX::Scene::DebugGUI()
{
	GameObject::DebugGUI();
}

void DirectX::Scene::UnLoad()
{
	
}