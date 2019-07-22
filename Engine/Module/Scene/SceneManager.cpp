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

//Scene�̕ۊǌ�
std::list<std::shared_ptr<Scene>> SceneManager::pSceneIndex;
//���݂�Scene
std::weak_ptr<Scene> SceneManager::pActiveScene;
//����Scene
std::weak_ptr<Scene> SceneManager::pNextScene;
//�J�ڂ��L����
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
//�C���X�^���X����
void SceneManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new SceneManager();
}
//�C���X�^���X�폜
void SceneManager::Destroy()
{
	if (pInstance == nullptr) return;
	delete pInstance;
}

//--- Scene Method --------------------------------------------------

//Scene������v���Ă���Γǂݍ���
void SceneManager::LoadScene(std::string SceneName)
{
	//Index����
	for (auto wp_Scene : pSceneIndex)
		if (wp_Scene->name == SceneName){
			LoadScene(wp_Scene);
			return;
		}
}

//Scene�ǂݍ���
void DirectX::SceneManager::LoadScene(std::weak_ptr<Scene> scene)
{
	if(pActiveScene.expired()){
		AttachActiveScene(scene);
		return;
	}
	//����Scene�֓o�^
	SetIsChangeScene(scene);
}

//ActiveScene��Component���փ��b�Z�[�W�𑗐M����
void DirectX::SceneManager::RunActiveScene(Component::Message message)
{
	for (std::shared_ptr<GameObject> gameObject : pActiveScene.lock()->GameObjectIndex)
		gameObject->RunComponent(message);
}

//ActiveScene���̓����蔻����X�V
void DirectX::SceneManager::OnTriggerUpdate()
{
	for (std::shared_ptr<GameObject> gameObject : pActiveScene.lock()->GameObjectIndex){
		if (!gameObject.get()->IsActive)continue;
		std::weak_ptr<Collider> collider = gameObject.get()->GetComponent<Collider>();
		if (collider.expired())continue;
		//Trigger�ł͂Ȃ�
		if (!collider.lock()->GetEnable() || !collider.lock()->GetTrigger()) continue;
		//����
		for(std::shared_ptr<GameObject> otherObject:pActiveScene.lock()->GameObjectIndex){
			if (gameObject == otherObject) continue;
		}
	}
}

//ActiveScene���̕��������蔻����X�V
void DirectX::SceneManager::OnCollisionUpdate()
{

}

//ActiveScene���I�u�W�F�N�g��ImGUI�\��
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

//�폜�w�肳�ꂽ�I�u�W�F�N�g���폜
void DirectX::SceneManager::CleanUp()
{
	//CeanUp���Ȃ��Ă���
	if (!pActiveScene._Get()->IsCeanUp) return;
	//�폜
	pActiveScene._Get()->GameObjectIndex.remove_if([](std::shared_ptr<GameObject> gameObject) { return gameObject->IsDestroy; });
	//CeanUp����
	pActiveScene._Get()->IsCeanUp = false;
}

//Scene�J��
void DirectX::SceneManager::ChangeScene()
{
	//�J�ڃt���O���L��
	if (!IsChangeScene) return;
	//ActiveScene������
	DetachActiveScene();
	//����Scene��ActiveScene�֕ύX
	AttachActiveScene(pNextScene);
	IsChangeScene = false;
}

//ActiveScene�̎擾
std::weak_ptr<Scene> DirectX::SceneManager::GetActiveScene()
{
	return pActiveScene;
}

//Scene������Scene���擾
std::weak_ptr<Scene> DirectX::SceneManager::GetSceneByName(std::string SceneName)
{
	for (auto wp_Scene : pSceneIndex)
		if (wp_Scene->name == SceneName)
			return wp_Scene;
	return std::weak_ptr<Scene>();
}

//ActiveScene��ݒ�
void DirectX::SceneManager::AttachActiveScene(std::weak_ptr<Scene> scene)
{
	pActiveScene.reset();
	pActiveScene = scene;
	scene.lock()->IsLoaded = true;
	scene.lock()->IsCeanUp = false;
	scene.lock()->Load();
	//����������
	RunActiveScene(Component::Initialize);
}

//ActiveScene��j��
void DirectX::SceneManager::DetachActiveScene()
{
	pActiveScene.lock()->IsLoaded = false;
	pActiveScene.lock()->IsCeanUp = false;
	//�I������
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

//GameObject��Tag�w��Œǉ�
GameObject* Scene::AddSceneObject(std::string name,TagName tag)
{
	std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject(name,this,tag));
	GameObjectIndex.push_back(object);
	object->self = object;
	object->transform->gameObject = object;
	return object.get();
}

//GameObject��DefaultTag�Ŏw��
GameObject* Scene::AddSceneObject(std::string name)
{
	return AddSceneObject(name, TagName::Default);
}

