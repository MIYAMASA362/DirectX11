#include<List>
#include<string>
#include"main.h"

#include"Transform.h"
#include"GameObject.h"
#include"SceneManager.h"

using namespace DirectX;

SceneManager* SceneManager::pInstance = NULL;

std::list<Scene*> SceneManager::pSceneIndex;
Scene* SceneManager::pActiveScene = NULL;

//--- SceneManager ------------------------------------------------------------

SceneManager::SceneManager()
{
	pSceneIndex.clear();
	pActiveScene = NULL;
}

SceneManager::~SceneManager()
{
	if (pActiveScene != NULL)
		pActiveScene = NULL;

	if (pSceneIndex.size() != 0)
	{
		for (Scene* scene : pSceneIndex)
			if (scene != NULL) delete scene;
		pSceneIndex.clear();
	}
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
	for (Scene* scene : pSceneIndex)
	{
		if (scene->name != SceneName) continue;
		if (scene->IsLoaded == true) break;
		pActiveScene = scene;
		scene->IsLoaded = true;
		break;
	}
}

void SceneManager::UnLoadScene(std::string SceneName)
{
	if (pActiveScene->name != SceneName) return;
	pActiveScene = NULL;
}

Scene* SceneManager::GetScene(std::string SceneNamae)
{
	for (Scene* scene : pSceneIndex)
		if (scene->name == SceneNamae) return scene;
	return NULL;
}

//--- SceneLoop ----------------------------------------------------------

void SceneManager::Initialize()
{
	if(pActiveScene != NULL)
		pActiveScene->Initialize();
}

void SceneManager::Update()
{
	if (pActiveScene != NULL)
		pActiveScene->Update();
}

void SceneManager::Render()
{
	if (pActiveScene != NULL)
		pActiveScene->Render();
}

void SceneManager::Finalize()
{
	if (pActiveScene != NULL)
		pActiveScene->Finalize();
}

//--- Scene -------------------------------------------------------------------

Scene::Scene(std::string name):name(name)
{

}

Scene::~Scene()
{
	for (GameObject* gameObject:pGameObjects)
		delete gameObject;
}

//--- Loop Method ---------------------------------------------------

void Scene::Initialize()
{
	for (GameObject* gameObject : pGameObjects)
		gameObject->Initialize();
}

void Scene::Update()
{
	for (GameObject* gameObject : pGameObjects)
		gameObject->Update();
}

void Scene::Render()
{
	for (GameObject* gameObject : pGameObjects)
		gameObject->Render();
}

void Scene::Finalize()
{
	for (GameObject* gameObject : pGameObjects)
		gameObject->Finalize();
}
