#include<List>
#include<string>
#include<memory>
#include<d3d11.h>
#include<DirectXMath.h>

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//Component
#include"Module\Object\Object.h"
#include"Module\Component\Component.h"

#include"Module\Tag\Tag.h"

#include"SceneManager.h"

#include"Module\Transform\Transform.h"
#include"Module\GameObject\GameObject.h"

#include"Module\Collision\Collision.h"

#include"../IMGUI/GUI_ImGui.h"

using namespace DirectX;

SceneManager* SceneManager::pInstance = nullptr;

//Sceneの保管庫
std::list<std::shared_ptr<Scene>> SceneManager::pSceneIndex;
//現在のScene
std::weak_ptr<Scene> SceneManager::pActiveScene;
//次のScene
std::weak_ptr<Scene> SceneManager::pNextScene;
//遷移が有効か
bool SceneManager::IsChangeScene = false;

//--- SceneManager ------------------------------------------------------------
SceneManager::SceneManager()
{
	pSceneIndex.clear();
}
SceneManager::~SceneManager()
{
	if (!pActiveScene.expired())
		pActiveScene.reset();
	if (!pNextScene.expired())
		pNextScene.reset();

	if (pSceneIndex.size() != 0)
		pSceneIndex.clear();
}

//--- Singleton Method ----------------------------------------------
//インスタンス生成
void SceneManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new SceneManager();
}
//インスタンス削除
void SceneManager::Destroy()
{
	if (pInstance == nullptr) return;
	delete pInstance;
}

//--- Scene Method --------------------------------------------------

//Scene名が一致していれば読み込み
void SceneManager::LoadScene(std::string SceneName)
{
	//Index検索
	for (auto wp_Scene : pSceneIndex)
		if (wp_Scene->name == SceneName){
			LoadScene(wp_Scene);
			return;
		}
}

//Scene読み込み
void DirectX::SceneManager::LoadScene(std::weak_ptr<Scene> scene)
{
	if(pActiveScene.expired()){
		AttachActiveScene(scene);
		return;
	}
	//次のSceneへ登録
	SetIsChangeScene(scene);
}

//ActiveSceneのComponentをへメッセージを送信する
void DirectX::SceneManager::RunActiveScene(Component::Message message)
{
	for (std::shared_ptr<GameObject> gameObject : pActiveScene.lock()->GameObjectIndex)
		gameObject->RunComponent(message);
}

//ActiveScene内の当たり判定を更新
void DirectX::SceneManager::OnTriggerUpdate()
{
	for (std::shared_ptr<GameObject> gameObject : pActiveScene.lock()->GameObjectIndex){
		if (!gameObject.get()->IsActive)continue;
		std::weak_ptr<Collider> collider = gameObject.get()->GetComponent<Collider>();
		if (collider.expired())continue;
		//Triggerではない
		if (!collider.lock()->GetEnable() || !collider.lock()->GetTrigger()) continue;
		//相手
		for(std::shared_ptr<GameObject> otherObject:pActiveScene.lock()->GameObjectIndex){
			if (gameObject == otherObject) continue;
		}
	}
}

//ActiveScene内の物理当たり判定を更新
void DirectX::SceneManager::OnCollisionUpdate()
{

}

//ActiveScene内オブジェクトのImGUI表示
void DirectX::SceneManager::DebugGUI_ActiveScene()
{
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

//削除指定されたオブジェクトを削除
void DirectX::SceneManager::CleanUp()
{
	//CeanUpしなくていい
	if (!pActiveScene._Get()->IsCeanUp) return;
	//削除
	pActiveScene._Get()->GameObjectIndex.remove_if([](std::shared_ptr<GameObject> gameObject) { return gameObject->IsDestroy; });
	//CeanUp完了
	pActiveScene._Get()->IsCeanUp = false;
}

//Scene遷移
void DirectX::SceneManager::ChangeScene()
{
	//遷移フラグが有効
	if (!IsChangeScene) return;
	//ActiveSceneを解除
	DetachActiveScene();
	//次のSceneをActiveSceneへ変更
	AttachActiveScene(pNextScene);
	IsChangeScene = false;
}

//ActiveSceneの取得
std::weak_ptr<Scene> DirectX::SceneManager::GetActiveScene()
{
	return pActiveScene;
}

//Scene名からSceneを取得
std::weak_ptr<Scene> DirectX::SceneManager::GetSceneByName(std::string SceneName)
{
	for (auto wp_Scene : pSceneIndex)
		if (wp_Scene->name == SceneName)
			return wp_Scene;
	return std::weak_ptr<Scene>();
}

//ActiveSceneを設定
void DirectX::SceneManager::AttachActiveScene(std::weak_ptr<Scene> scene)
{
	pActiveScene.reset();
	pActiveScene = scene;
	scene.lock()->IsLoaded = true;
	scene.lock()->IsCeanUp = false;
	scene.lock()->Load();
	//初期化処理
	RunActiveScene(Component::Initialize);
}

//ActiveSceneを破棄
void DirectX::SceneManager::DetachActiveScene()
{
	pActiveScene.lock()->IsLoaded = false;
	pActiveScene.lock()->IsCeanUp = false;
	//終了処理
	RunActiveScene(Component::Finalize);
	pActiveScene.lock()->GameObjectIndex.clear();
	pActiveScene.reset();
}

//--- Scene -------------------------------------------------------------------

Scene::Scene(std::string name):name(name)
{
	GameObjectIndex.clear();
}

Scene::~Scene()
{
	GameObjectIndex.clear();
}

//GameObjectをTag指定で追加
GameObject* Scene::AddSceneObject(std::string name,TagName tag)
{
	std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject(name,this,tag));
	GameObjectIndex.push_back(object);
	object->self = object;
	object->transform->gameObject = object;
	return object.get();
}

//GameObjectをDefaultTagで指定
GameObject* Scene::AddSceneObject(std::string name)
{
	return AddSceneObject(name, TagName::Default);
}

