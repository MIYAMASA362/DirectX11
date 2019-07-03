#include<List>
#include<string>
#include"main.h"

#include"DirectXStruct.h"
#include"Object.h"
#include"Component.h"
#include"Transform.h"
#include"Tag.h"
#include"Renderer.h"
#include"GameObject.h"
#include"Behaviour.h"
#include"camera.h"
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

void DirectX::SceneManager::RunActiveScene(Component::Message message)
{
	if (!pActiveScene.expired())
		for (std::shared_ptr<GameObject> gameObject : pActiveScene.lock()->GameObjectIndex)
			for (std::shared_ptr<Component> component : gameObject->Components) {
				if (!component->GetEnable()) continue;
				component->gameObject = gameObject;
				component->transform = gameObject->transform;
				component->SendBehaviourMessage(message);
			}
}

void DirectX::SceneManager::CleanUp()
{
	if (pActiveScene.expired()) return;
	if (!pActiveScene._Get()->IsCeanUp) return;
	pActiveScene._Get()->GameObjectIndex.remove_if([](std::shared_ptr<GameObject> gameObject) { return gameObject->IsDestroy; });
	pActiveScene._Get()->IsCeanUp = false;
}

//--- Scene -------------------------------------------------------------------

Scene::Scene(std::string name):name(name)
{

}

Scene::~Scene()
{
	GameObjectIndex.clear();
}

GameObject* Scene::AddSceneObject(std::string name,TagName tag)
{
	std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject(name,this,tag));
	GameObjectIndex.push_back(object);
	object->self = object;
	return object.get();
}

GameObject* Scene::AddSceneObject(std::string name)
{
	return AddSceneObject(name, TagName::Default);
}


