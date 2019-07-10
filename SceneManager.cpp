#include<List>
#include<string>
#include"main.h"

#include"GUI_ImGui.h"
#include"DirectXStruct.h"
#include"DirectX.h"
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
	if (pActiveScene.expired()) return;
	for (std::shared_ptr<GameObject> gameObject : pActiveScene.lock()->GameObjectIndex)
		gameObject->RunComponent(message);
}

void DirectX::SceneManager::DebugGUI_ActiveScene()
{
	if (pActiveScene.expired()) return;
	
	std::string label = "Scene:";
	label += pActiveScene.lock()->name;
	ImGui::Begin(label.c_str());
	
	for(auto gameObject : pActiveScene.lock()->GameObjectIndex)
	{
		ImGui::SetNextTreeNodeOpen(false,ImGuiCond_Once);
		if(ImGui::TreeNode(gameObject->name.c_str())){
			Vector3 position = gameObject->transform.get()->position();
			Vector3 rotation = Quaternion::ToEulerAngles(gameObject->transform.get()->rotation());
			Quaternion q = gameObject->transform.get()->rotation();
			Vector3 scale = gameObject->transform.get()->scale();
			ImGui::InputFloat3("Position",&position.x);
			ImGui::InputFloat3("Rotation",&rotation.x);
			ImGui::InputFloat3("Scale",&scale.x);

			if(ImGui::TreeNode("Component")){
				for(auto component : gameObject->Components){
					ImGui::Text(component->ObjectName().c_str());
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
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
	object->transform->gameObject = object;
	return object.get();
}

GameObject* Scene::AddSceneObject(std::string name)
{
	return AddSceneObject(name, TagName::Default);
}


