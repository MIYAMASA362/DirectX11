#define INCLUDE_CEREAL
#include"Common.h"

//DirectX
#include"Module\DirectX\DirectX.h"

//ECS
#include"Module\ECSEngine.h"

#include"Module\Tag\Tag.h"
#include"Module\Transform\Transform.h"
#include"Module\GameObject\GameObject.h"

#include"Module\Hierarchy\Hierarchy.h"

#include"Scene.h"
#include"SceneManager.h"


#include"../IMGUI/GUI_ImGui.h"

#include"Module\Camera\camera.h"

using namespace DirectX;

//*********************************************************************************************************************
//
//	SceneManager
//
//*********************************************************************************************************************

//インスタンス
SceneManager* SceneManager::pInstance = nullptr;

//SceneManager
//	コンストラクタ
//
SceneManager::SceneManager()
	:
	_IsChangeScene(false)
{

}

//~SceneManager
//	デストラクタ
//
SceneManager::~SceneManager()
{
	_ActiveScene.reset();
	_NextScene.reset();
}



//Create
//	インスタンス生成
//
void SceneManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new SceneManager();
}

//Destroy
//	インスタンス削除
//
void SceneManager::Destroy()
{
	if (pInstance == nullptr) return;
	delete pInstance;
	pInstance = nullptr;
}



//CreateScene
//	シーン生成
//
std::weak_ptr<Scene> SceneManager::CreateScene(std::string name)
{
	char path[MAX_PATH + 1];
	GetCurrentDirectory(ARRAYSIZE(path),path);
	strcat_s(path,(name + ".scene").data());

	auto result = std::shared_ptr<Scene>(new Scene(name, path));
	
	auto camera = result->AddSceneObject("MainCamera");
	camera->AddComponent<Camera>();
	auto transform = camera->transform();

	_SceneArray.push_back(result);
	return result;
}

//LoadScene
//	シーン読み込み
//
void SceneManager::LoadScene(std::weak_ptr<Scene> scene)
{
	//ActiveSceneが無い
	if (_ActiveScene.expired())
	{
		AttachActiveScene(scene);
		return;
	}
	//次のSceneへ登録
	SetIsChangeScene(scene);
}

void SceneManager::LoadScene(std::string name)
{
	LoadScene(GetSceneByName(name));
}



//ChangeScene
//	シーン遷移
//
void SceneManager::ChangeScene()
{
	if (!_IsChangeScene) return;

	DetachActiveScene();

	AttachActiveScene(_NextScene.lock());

	_IsChangeScene = false;
}

//GetSceneByName
//	Scene名からSceneを取得
//
std::weak_ptr<Scene> SceneManager::GetSceneByName(std::string SceneName)
{
	std::shared_ptr<Scene> find;
	for (auto scene : _SceneArray)
		if (scene->CompareName(SceneName))
		{
			find = scene;
			break;
		}
	return find;
}



//SetIsChangeScene
//	次のSceneを設定
//
void SceneManager::SetIsChangeScene(std::weak_ptr<Scene> scene)
{
	_NextScene = scene;
	_IsChangeScene = true;
}

//AttachActiveScene
//	ActiveSceneを設定
//
void SceneManager::AttachActiveScene(std::weak_ptr<Scene> scene)
{
	_ActiveScene = scene;
	_ActiveScene.lock()->AttachActiveScene();
}

//DetachActiveScene
//	ActiveSceneを破棄
//
void SceneManager::DetachActiveScene()
{
	_ActiveScene.lock()->DetachActiveScene();
	_ActiveScene.reset();
}



//DebugGUI_ActiveScene
//	オブジェクトのImGUI表示
//
void SceneManager::DebugGUI_ActiveScene()
{
	std::string label = "Scene:";
	label += _ActiveScene.lock()->GetSceneName();
	ImGui::Begin("Scene");
	ImGui::Text(label.c_str());
	_ActiveScene.lock()->DebugGUI();
	ImGui::End();
}


