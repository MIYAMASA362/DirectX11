#pragma once


namespace DirectX
{
	class Scene;
	class GameObject;

	/*
	�݌v�F
		ActiveScene�͕K�����݂���(���݂��Ȃ��ꍇ��nullptr�Ƃ��ė�O���N�����ׂ�)
	*/

	//=== Scene�Ǘ� ===========================================================
	class SceneManager
	{
	private:
		static SceneManager* pInstance;
		//Scene�̕ۊǌ�
		static std::list<std::shared_ptr<Scene>> pSceneIndex;
		//���݂�Scene
		static std::weak_ptr<Scene> pActiveScene;
		//����Scene
		static std::weak_ptr<Scene> pNextScene;
		static bool IsChangeScene;
	private:
		SceneManager();
		~SceneManager();
	//--- Singleton -------------------------------------------------
	public:
		static void Create();			//�C���X�^���X����
		static void Destroy();			//�C���X�^���X�폜

	//--- Scene -----------------------------------------------------
	public:
		//Scene�̐���
		template<typename Type>
		static void CreateScene()
		{
			std::shared_ptr<Scene> AddScene = std::shared_ptr<Scene>(new Type());
			pSceneIndex.push_back(AddScene);
			AddScene->self = AddScene;
			return;
		};
	public:
		//Scene�̓ǂݍ���
		static void LoadScene(std::string SceneName);
		//Scene�̓ǂݍ���
		static void LoadScene(std::weak_ptr<Scene> scene);	
		//ActiveScene�փ��b�Z�[�W�𑗐M
		static void RunActiveScene(Component::Message message);
		//ActiveScene�̓����蔻����X�V
		static void OnTriggerUpdate();
		//ActiveScene�̕��������蔻����X�V
		static void OnCollisionUpdate();
		//ActiveScene���I�u�W�F�N�g��ImGUI�\��
		static void DebugGUI_ActiveScene();
		//ActiveScene���ō폜�w�肳�ꂽ�I�u�W�F�N�g���폜
		static void CleanUp();
		//ActiveScene��NextScene��؂�ւ�
		static void ChangeScene();
		//GetActiveScene
		static std::weak_ptr<Scene>GetActiveScene();
		//GetSceneByName
		static std::weak_ptr<Scene>GetSceneByName(std::string SceneName);

	//--- �������� --------------------------------------------------
	private:
		//SetIsChangeScene
		static void SetIsChangeScene(std::weak_ptr<Scene> scene)
		{
			pNextScene = scene;
			IsChangeScene = true;
		}
		//SetActiveScene
		static void AttachActiveScene(std::weak_ptr<Scene> scene);
		//DitachActiveScene
		static void DetachActiveScene();
	};

	//=== Scene ==================================================================
	class Scene
	{
		friend class SceneManager;
	private:
		const std::string name;
		bool IsLoaded = false;
		bool IsCeanUp = false;
	private:
		std::weak_ptr<Scene> self;	//���g�ւ̎Q��
		std::list<std::shared_ptr<GameObject>> GameObjectIndex;
	protected:
		Scene(std::string name);
		virtual ~Scene();
	public:
		//Scene���ǂݍ��܂ꂽ��
		virtual void Load() = 0;
		//�Q�[���I�u�W�F�N�g�̐���
		GameObject* AddSceneObject(std::string name, TagName tag);
		GameObject* AddSceneObject(std::string name);
		//�Q�[���I�u�W�F�N�g�̍폜������\��
		void SetIsCeanUp(bool IsEnable) { this->IsCeanUp = IsEnable; };
	};
}