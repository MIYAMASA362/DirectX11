#include<List>
#include<string>
#include"main.h"

#include"DirectXStruct.h"
#include"Transform.h"
#include"Object.h"
#include"Component.h"
#include"Tag.h"
#include"GameObject.h"
#include"SceneManager.h"

using namespace DirectX;

SceneManager* SceneManager::pInstance = NULL;

std::list<std::shared_ptr<Scene>> SceneManager::pSceneIndex;
std::weak_ptr<Scene> SceneManager::pActiveScene;

//--- SceneManager ------------------------------------------------------------

SceneManager::SceneManager()
{
	pSceneIndex.clear();
}

SceneManager::~SceneManager()
{
	if (!pActiveScene.expired())
		pActiveScene.reset();

	if (pSceneIndex.size() != 0)
		pSceneIndex.clear();
}

//--- Singleton Method ----------------------------------------------

void SceneManager::Create()
{
	if (pInstance != NULL) return;
	pInstance = new SceneManager();
}

void SceneManager::Destroy()
{
	if (pInstance == NULL) return;
	delete pInstance;
}

//--- Scene Method --------------------------------------------------

void SceneManager::LoadScene(std::string SceneName)
{
	for (auto wp_Scene : pSceneIndex)
	{
		if (wp_Scene->name != SceneName) continue;
		if (wp_Scene->IsLoaded == true) break;
		pActiveScene = wp_Scene;
		pActiveScene.lock();
		wp_Scene->IsLoaded = true;
		break;
	}
}

void SceneManager::UnLoadScene(std::string SceneName)
{
	if (pActiveScene._Get()->name != SceneName) return;
	pActiveScene.reset();
}

Scene* SceneManager::GetScene(std::string SceneNamae)
{
	for (auto wp_Scene : pSceneIndex)
		if (wp_Scene->name == SceneNamae) return &(*wp_Scene);
	return NULL;
}

//--- SceneLoop ----------------------------------------------------------

void SceneManager::Initialize()
{
	if(!pActiveScene.expired())
		pActiveScene._Get()->Initialize();
}

void SceneManager::Update()
{
	if (!pActiveScene.expired())
		pActiveScene._Get()->Update();
}

void SceneManager::Render()
{
	if (!pActiveScene.expired())
		pActiveScene._Get()->Render();
}

void SceneManager::Finalize()
{
	if (!pActiveScene.expired())
		pActiveScene._Get()->Finalize();
}

void DirectX::SceneManager::CleanUp()
{
	if (pActiveScene.expired()) return;
	if (!pActiveScene._Get()->GetIsCeanUp()) return;
	pActiveScene._Get()->GameObjectIndex.remove_if([](std::shared_ptr<GameObject> gameObject) { return gameObject->GetIsDestroy(); });
	pActiveScene._Get()->SetIsCeanUp(false);
}

//--- Scene -------------------------------------------------------------------

Scene::Scene(std::string name):name(name)
{

}

Scene::~Scene()
{
	GameObjectIndex.clear();
}

//--- Loop Method ---------------------------------------------------

void Scene::Initialize()
{
	for (std::shared_ptr<GameObject> gameObject : GameObjectIndex)
		gameObject->Initialize();
}

void Scene::Update()
{
	for (std::shared_ptr<GameObject> gameObject : GameObjectIndex)
		gameObject->Update();
}

void Scene::Render()
{
	for (std::shared_ptr<GameObject> gameObject : GameObjectIndex)
		gameObject->Render();
}

void Scene::Finalize()
{
	for (std::shared_ptr<GameObject> gameObject : GameObjectIndex)
		gameObject->Finalize();
}

GameObject* Scene::AddSceneObject(std::string name, TagManager::TagName tag)
{
	std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject(name, this, tag));
	GameObjectIndex.push_back(object);
	return object.get();
}

GameObject* Scene::AddSceneObject(std::string name)
{
	return AddSceneObject(name, TagManager::Default);
}