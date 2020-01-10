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

//SceneID�̃J�E���g
SceneID SceneManager::m_SceneID;
//Scene�̕ۊǌ�
std::map<SceneID, std::shared_ptr<Scene>> SceneManager::pSceneDictionary;
//���݂�Scene
std::weak_ptr<Scene> SceneManager::pActiveScene;
//����Scene
std::weak_ptr<Scene> SceneManager::pNextScene;
//�J�ڂ��L����
bool SceneManager::IsChangeScene = false;

//Destroy
//	Scene�폜
//
void SceneManager::Destroy()
{
	if (!pActiveScene.expired())
		pActiveScene.reset();
	if (!pNextScene.expired()) 
		pNextScene.reset();

	pSceneDictionary.clear();
}

//Scene�ǂݍ���
void SceneManager::LoadScene(std::weak_ptr<Scene> scene)
{
	//ActiveScene������
	if(pActiveScene.expired())
		return AttachActiveScene(scene);

	//����Scene�֓o�^
	SetIsChangeScene(scene);
}

//ActiveScene���I�u�W�F�N�g��ImGUI�\��
void SceneManager::DebugGUI_ActiveScene()
{
	std::string label = "Scene:";
	label += pActiveScene.lock()->GetSceneName();
	ImGui::Begin("Scene");
	ImGui::Text(label.c_str());
	pActiveScene.lock()->DebugGUI();
	ImGui::End();
}

//Scene�J��
void SceneManager::ChangeScene()
{
	if (!IsChangeScene) return;	//�J�ڃt���O���L��
	//������
	pNextScene = pNextScene;
	DetachActiveScene();
	//�L����
	pNextScene = pNextScene;
	AttachActiveScene(pNextScene.lock());

	ObjectManager::ClearnUp();
	
	ComponentManager::SendComponentMessage("Start");

	IsChangeScene = false;
}

//ActiveScene�̎擾
std::weak_ptr<Scene> SceneManager::GetActiveScene()
{
	return pActiveScene;
}

std::weak_ptr<Scene> SceneManager::GetScene(SceneID id)
{
	return pSceneDictionary.at(id);
}

//Scene������Scene���擾
std::weak_ptr<Scene> SceneManager::GetSceneByName(std::string SceneName)
{
	for (auto scene : pSceneDictionary) {
		if (scene.second->CompareName(SceneName))
			return scene.second;
	}
	return std::shared_ptr<Scene>(nullptr);
}

//����Scene��ݒ�
void SceneManager::SetIsChangeScene(std::weak_ptr<Scene> scene)
{
	pNextScene = scene;
	IsChangeScene = true;
}

//ActiveScene��ݒ�
void SceneManager::AttachActiveScene(std::weak_ptr<Scene> scene)
{
	pActiveScene = scene;
	pActiveScene.lock()->AttachActiveScene();
}

//ActiveScene��j��
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
//	�R���X�g���N�^
//
Scene::Scene(std::string name)
	:
	m_id(SceneManager::AttachID()),
	m_name(name),
	_hierarchyUtility(new HierarchyUtility())
{

};

//~Scene
//	�f�X�g���N�^
//
inline Scene::~Scene()
{
	delete _hierarchyUtility;
};

//AddSceneObject
//	Scene�ɃI�u�W�F�N�g��ǉ�����
//
GameObject* Scene::AddSceneObject(std::string name,TagName tag)
{
	auto instance = new GameObject(name, this, tag);

	//�K�w�ǉ�
	_hierarchyUtility->AttachHierarchy(instance->GetEntityID());

	//Transform
	instance->AddComponent<Transform>();
	return instance;
}

//RemoveSceneObject
//	Scene����I�u�W�F�N�g�폜
//
void Scene::RemoveSceneObject(EntityID id)
{
	_hierarchyUtility->DetachHierarchy(id);
}

//AttachActiveScene
//	�A�N�e�B�u�ȃV�[���ɂ���
//
void Scene::AttachActiveScene()
{
	this->IsLoaded = true;
	this->Load();	//�ǂݍ���
}

//DetachActiveScene
//	��A�N�e�B�u�ȃV�[���ɂ���
//
void Scene::DetachActiveScene()
{
	this->IsLoaded = false;
	this->UnLoad();
}

static GameObject* Debug_InspectorObject = nullptr;

//DebugGUI_SceneHierarchy
//	Hierarchy��Debug�\��
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
//	�f�o�b�O�\��
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