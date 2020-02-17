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

//�C���X�^���X
SceneManager* SceneManager::pInstance = nullptr;

//SceneManager
//	�R���X�g���N�^
//
SceneManager::SceneManager()
	:
	_IsChangeScene(false)
{
	//Array�m��
	_SceneArray.reserve(10);
}

//~SceneManager
//	�f�X�g���N�^
//
SceneManager::~SceneManager()
{
	_ActiveScene.reset();
	_NextScene.reset();
}



//Create
//	�C���X�^���X����
//
void SceneManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new SceneManager();
}

//Destroy
//	�C���X�^���X�폜
//
void SceneManager::Destroy()
{
	if (pInstance == nullptr) return;
	delete pInstance;
	pInstance = nullptr;
}



//CreateScene
//	�V�[������
//
void SceneManager::CreateScene(std::string name)
{
	//�t�@�C��
	char path[MAX_PATH + 1];
	GetCurrentDirectory(ARRAYSIZE(path),path);
	strcat_s(path,("\\"+ name + ".scene").data());

	//Scene����
	auto scene = RegisterScene(new Scene(name, path));
	
	//�I�u�W�F�N�g����
	auto camera = scene->AddSceneObject("MainCamera");
	camera->AddComponent<Camera>();

	//�Z�[�u
	scene->Save();

	//�j��
	camera->Destroy();

}

//RegisterScene
//	SceneArray�փC���X�^���X�o�^
//
std::shared_ptr<Scene> SceneManager::RegisterScene(Scene * scene)
{
	auto result = std::shared_ptr<Scene>(scene);
	_SceneArray.push_back(result);
	return result;
}

//LoadScene
//	�V�[���ǂݍ���
//
void SceneManager::LoadScene(std::weak_ptr<Scene> scene)
{
	//ActiveScene������
	if (_ActiveScene.expired())
	{
		AttachActiveScene(scene);
		return;
	}

	//����Scene�֓o�^
	SetIsChangeScene(scene);
}

void SceneManager::LoadScene(std::string name)
{
	LoadScene(GetSceneByName(name));
}

void SceneManager::SaveScene(std::weak_ptr<Scene> load)
{
	load.lock()->Save();
}



//ChangeScene
//	�V�[���J��
//
void SceneManager::ChangeScene()
{
	if (!_IsChangeScene) return;

	DetachActiveScene();

	ObjectManager::GetInstance()->ClearnUpObject();

	AttachActiveScene(_NextScene.lock());

	_IsChangeScene = false;
}

//GetSceneByName
//	Scene������Scene���擾
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
//	����Scene��ݒ�
//
void SceneManager::SetIsChangeScene(std::weak_ptr<Scene> scene)
{
	_NextScene = scene;
	_IsChangeScene = true;
}

//AttachActiveScene
//	ActiveScene��ݒ�
//
void SceneManager::AttachActiveScene(std::weak_ptr<Scene> scene)
{
	_ActiveScene = scene;
	_ActiveScene.lock()->AttachActiveScene();
}

//DetachActiveScene
//	ActiveScene��j��
//
void SceneManager::DetachActiveScene()
{
	_ActiveScene.lock()->DetachActiveScene();
	_ActiveScene.reset();
}



//DebugGUI_ActiveScene
//	�I�u�W�F�N�g��ImGUI�\��
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


