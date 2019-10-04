#include<List>
#include<map>
#include<typeinfo>
#include<string>
#include<memory>
#include<vector>
#include<d3d11.h>
#include<DirectXMath.h>

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

//�C���X�^���X�폜
void SceneManager::Destroy()
{
	if (!pActiveScene.expired())
		pActiveScene.reset();
	if (!pNextScene.expired())
		pNextScene.reset();

	pSceneDictionary.clear();
}

//Scene�ǂݍ���
void DirectX::SceneManager::LoadScene(std::weak_ptr<Scene> scene)
{
	//ActiveScene������
	if(pActiveScene.expired())
		return AttachActiveScene(scene);

	//����Scene�֓o�^
	SetIsChangeScene(scene);
}

//ActiveScene���I�u�W�F�N�g��ImGUI�\��
void DirectX::SceneManager::DebugGUI_ActiveScene()
{
	std::string label = "Scene:";
	label += pActiveScene.lock()->GetSceneName();
	ImGui::Begin("Scene");
	ImGui::Text(label.c_str());
	pActiveScene.lock()->DebugGUI();
	ImGui::End();
}

//Scene�J��
void DirectX::SceneManager::ChangeScene()
{
	
	if (!IsChangeScene) return;	//�J�ڃt���O���L��
	//������
	DetachActiveScene();
	//�L����
	AttachActiveScene(pNextScene);
	IsChangeScene = false;
}

//ActiveScene�̎擾
std::weak_ptr<Scene> DirectX::SceneManager::GetActiveScene()
{
	return pActiveScene;
}

std::weak_ptr<Scene> DirectX::SceneManager::GetScene(SceneID id)
{
	return pSceneDictionary.at(id);
}

//Scene������Scene���擾
std::weak_ptr<Scene> DirectX::SceneManager::GetSceneByName(std::string SceneName)
{
	auto itr = pSceneDictionary.begin();
	auto end = pSceneDictionary.end();
	for (; itr != end; itr++)
		if (itr->second.get()->CompareName(SceneName))
			return itr->second;
	return std::shared_ptr<Scene>(nullptr);
}

//����Scene��ݒ�
void DirectX::SceneManager::SetIsChangeScene(std::weak_ptr<Scene> scene)
{
	pNextScene = scene;
	IsChangeScene = true;
}

//ActiveScene��ݒ�
void DirectX::SceneManager::AttachActiveScene(std::weak_ptr<Scene> scene)
{
	pActiveScene = scene;
	pActiveScene.lock()->AttachActiveScene();
}

//ActiveScene��j��
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

//GameObject��Tag�w��Œǉ�
GameObject* DirectX::Scene::AddSceneObject(std::string name,TagName tag)
{
	auto instance = new GameObject(name, this, tag);
	this->Index.push_back(instance->GetEntityID());
	return instance;
}

//GameObejct�̍폜
void DirectX::Scene::RemoveSceneObject(EntityID id)
{
	EntityManager::RemoveEntity(id);
}

//
void DirectX::Scene::AttachActiveScene()
{
	this->IsLoaded = true;
	this->Load();	//�ǂݍ���
}

//
void DirectX::Scene::DetachActiveScene()
{
	this->IsLoaded = false;
	this->~Scene();
}

//ImGui Debug�\��
void DirectX::Scene::DebugGUI()
{
	GameObject::DebugGUI();
}

void DirectX::Scene::UnLoad()
{
	
}