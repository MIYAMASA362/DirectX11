#pragma once

/*
�݌v�F
�ESceneManager
	ActiveScene�͕K�����݂���(���݂��Ȃ��ꍇ��nullptr�Ƃ��ė�O���N�����ׂ�)
*/

class Scene;
class GameObject;
class HierarchyUtility;

//*********************************************************************************************************************
//
//	SceneMaanger
//		Scnee�Ǘ�
//
//*********************************************************************************************************************
class SceneManager final
{
	friend Scene;
private:
	//�V���O���C���X�^���X
	static SceneManager* pInstance;

	//�V�[���ۊ�
	std::vector<std::shared_ptr<Scene>> _SceneArray;

	//���݂̃V�[��
	std::weak_ptr<Scene> _ActiveScene;
	//�J�ڐ�V�[��
	std::weak_ptr<Scene> _NextScene;
	//�J�ڃt���O
	bool _IsChangeScene;


	//�R���X�g���N�^
	SceneManager();
	//�f�X�g���N�^
	~SceneManager();


public:

	//�C���X�^���X�擾
	static SceneManager* GetInstance() { return pInstance; };

	//�C���X�^���X����
	static void Create();
	//�C���X�^���X�j��
	static void Destroy();

	//�V�[������
	std::weak_ptr<Scene> CreateScene(std::string name);

	//�V�[���ǂݍ���
	void LoadScene(std::weak_ptr<Scene> scene);
	void LoadScene(std::string name);

	//���݂̃V�[���擾
	std::weak_ptr<Scene>GetActiveScene() { return _ActiveScene; }
	//�J�ڐ�V�[���̎擾
	std::weak_ptr<Scene>GetNextScene() { return _NextScene; }

	void ChangeScene();

	//�V�[�����Ŏ擾
	std::weak_ptr<Scene>GetSceneByName(std::string SceneName);

	bool IsChangeScene() { return _IsChangeScene; };

	//���݂̃V�[����ݒ�
	void AttachActiveScene(std::weak_ptr<Scene> scene);
	//�J�ڐ��ݒ�
	void SetIsChangeScene(std::weak_ptr<Scene> scene);
	//���݂̃V�[����j��
	void DetachActiveScene();


public:
	//�f�o�b�O�\��
	void DebugGUI_ActiveScene();


};