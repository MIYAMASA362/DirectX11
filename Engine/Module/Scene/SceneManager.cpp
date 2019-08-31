#include<List>
#include<map>
#include<typeinfo>
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
#include"Module\Behaviour\Behaviour.h"

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

//Sceneの保管庫
std::map<std::string, std::shared_ptr<Scene>> SceneManager::pSceneDictionary;
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

//--- Scene Method --------------------------------------------------

//Scene名が一致していれば読み込み
void SceneManager::LoadScene(std::string SceneName)
{
	LoadScene(pSceneDictionary.at(SceneName));
}

//Scene読み込み
void DirectX::SceneManager::LoadScene(std::weak_ptr<Scene> scene)
{
	//ActiveSceneが無い
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
	pActiveScene.lock()->SendComponentMessage(message);
}

//	RunActiveScene_Render()
//	
//	Canvasがアタッチされている場合、そのオブジェクトと子を2D描画として扱う
//
void DirectX::SceneManager::RunActiveScene_Render()
{
	pActiveScene.lock()->Render();
}

//ActiveSceneのGameObjectのColliderの更新
void DirectX::SceneManager::ColliderUpdate()
{
	pActiveScene.lock()->ColliderUpdate();
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

//削除指定されたオブジェクトを削除
void DirectX::SceneManager::CleanUp()
{
	pActiveScene.lock()->CleanUp();
}

//Scene遷移
void DirectX::SceneManager::ChangeScene()
{
	
	if (!IsChangeScene) return;	//遷移フラグが有効
	//無効化
	{
		RunActiveScene(Component::Finalize);
		DetachActiveScene();
	}
	//有効化
	{
		AttachActiveScene(pNextScene);
		RunActiveScene(Component::Initialize);
	}
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
	return pSceneDictionary.at(SceneName);
}

void DirectX::SceneManager::ApplyRigidbody()
{
	pActiveScene.lock()->ApplyRigidbody();
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

//--- Scene -------------------------------------------------------------------

//GameObjectをTag指定で追加
GameObject* Scene::AddSceneObject(std::string name,TagName tag)
{
	//GameObjectの生成
	std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject(name,this,tag));
	GameObjectIndex.push_back(object);
	object->self = object;
	object->AddComponent<Transform>();
	return object.get();
}

void Scene::SetIsCleanUp() {
	this->IsCleanUp = true;
}

void DirectX::Scene::SendComponentMessage(Component::Message message)
{
	for(auto gameObject:GameObjectIndex)
		gameObject->RunComponent(message);
}

void DirectX::Scene::AttachActiveScene()
{
	this->IsLoaded = true;
	this->IsCleanUp = false;
	this->Load();	//読み込み
}

void DirectX::Scene::DetachActiveScene()
{
	this->IsLoaded = false;
	this->IsCleanUp = false;
	this->GameObjectIndex.clear();
}

void DirectX::Scene::CleanUp()
{
	if (this->IsCleanUp) return;
	this->GameObjectIndex.remove_if([](std::shared_ptr<GameObject> gameObject) { return gameObject->IsDestroy; });
	this->IsCleanUp = false;
}

void DirectX::Scene::Render()
{
	//2D描画するオブジェクトのインデックス
	std::list<std::weak_ptr<GameObject>> RenderIndex_2D;

	//3D描画
	{
		for (auto gameObject :this->GameObjectIndex) {
			//アクティブでない
			if (!gameObject->IsActive) continue;
			//Canvasがアタッチされている
			if (gameObject->canvas) {
				RenderIndex_2D.push_back(gameObject);
				continue;
			}
			//親がいない
			if (gameObject->transform->GetParent().expired()) {
				gameObject->RunComponent(Component::Render);
				gameObject->transform->SendComponentMessageChildren(Component::Render);
			}
		}
	}

	//2D描画
	{
		//2D空間(スクリーン空間)に変換
		D3DApp::Renderer::SetWorldViewProjection2D();

		//2D描画
		for (auto gameObject : RenderIndex_2D)
			//親がいない
			if (gameObject.lock()->transform->GetParent().expired()) {
				gameObject.lock()->RunComponent(Component::Render);
				gameObject.lock()->transform->SendComponentMessageChildren(Component::Render);
			}
	}
}

void DirectX::Scene::ColliderUpdate()
{
	for(auto gameObject:this->GameObjectIndex){
		if (!gameObject->IsActive)continue;
		for (auto otherObject : this->GameObjectIndex) {
			if (gameObject == otherObject) continue;
			if (!otherObject->IsActive) continue;
			Collider::Hitjudgment(gameObject.get(),otherObject.get());
		}
	}
}

void DirectX::Scene::ApplyRigidbody()
{
	for(auto gameObject:this->GameObjectIndex){
		if (!gameObject->IsActive) continue;
		if (!gameObject->rigidbody) continue;
		if (!gameObject->rigidbody->GetEnable()) continue;
		gameObject->rigidbody->ApplyRigidbody();
	}
}

void DirectX::Scene::DebugGUI()
{
	for (auto gameObject : this->GameObjectIndex) {
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if(ImGui::TreeNode(gameObject->name.c_str())){
			Vector3 position = gameObject->transform.get()->position();
			Vector3 rotation = Quaternion::ToEulerAngles(gameObject->transform.get()->rotation());
			Quaternion q = gameObject->transform.get()->rotation();
			Vector3 scale = gameObject->transform.get()->scale();
			ImGui::InputFloat3("Position", &position.x);
			ImGui::InputFloat3("Rotation", &rotation.x);
			ImGui::InputFloat3("Scale", &scale.x);

			if (ImGui::TreeNode("Component")) {
				for (auto component : gameObject->Components) {
					component->DebugImGui();
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}