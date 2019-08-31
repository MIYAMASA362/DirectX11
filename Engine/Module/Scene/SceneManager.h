#pragma once

namespace DirectX
{
	class Scene;
	class GameObject;

	/*
	設計：
		ActiveSceneは必ず存在する(存在しない場合はnullptrとして例外を起こすべき)
	*/

	//=== Scene管理 ===========================================================
	class SceneManager:public SystemManager
	{
	private:
		//Sceneの保管庫
		static std::map<std::string, std::shared_ptr<Scene>> pSceneDictionary;
		//現在のScene
		static std::weak_ptr<Scene> pActiveScene;
		//次のScene
		static std::weak_ptr<Scene> pNextScene;
		//Scene遷移フラグ
		static bool IsChangeScene;
	public:
		SceneManager() = default;
		~SceneManager() = default;
	public:
		static void Destroy();
	//--- Scene処理 -------------------------------------------------
	public:
		//Sceneの生成
		template<typename Type> static void CreateScene()
		{
			std::shared_ptr<Scene> AddScene = std::shared_ptr<Scene>(new Type());
			pSceneDictionary.emplace(AddScene->GetSceneName(),AddScene);
			AddScene->SetSelfScene(AddScene);
			return;
		};
	public:
		virtual void initialize();
		virtual void update();
		virtual void render();
		virtual void finalize();
	public:
		static void LoadScene(std::string SceneName);
		static void LoadScene(std::weak_ptr<Scene> scene);
		static void RunActiveScene(Component::Message message);
		static void RunActiveScene_Render();
		static void ColliderUpdate();
		static void DebugGUI_ActiveScene();
		static void CleanUp();
		static void ChangeScene();
		static std::weak_ptr<Scene>GetActiveScene();
		static std::weak_ptr<Scene>GetSceneByName(std::string SceneName);
		static void ApplyRigidbody();
	private:
		static void SetIsChangeScene(std::weak_ptr<Scene> scene);
		static void AttachActiveScene(std::weak_ptr<Scene> scene);
		static void DetachActiveScene();
	};

	inline void SceneManager::initialize() {};
	inline void SceneManager::update() {
		RunActiveScene(Component::Update);
		RunActiveScene(Component::FixedUpdate);
		ApplyRigidbody();
		ColliderUpdate();
	};
	inline void SceneManager::render() {
		RunActiveScene_Render();
	};
	inline void SceneManager::finalize() {
		SceneManager::Destroy();
	};

	//=== Scene ==================================================================
	class Scene
	{
	private:
		const std::string name;
		bool IsLoaded = false;
		bool IsCleanUp = false;
	private:
		std::weak_ptr<Scene> self;
		std::list<std::shared_ptr<GameObject>> GameObjectIndex;
	protected:
		Scene(std::string name);
		virtual ~Scene();
	public:
		bool CompareName(std::string name);
		virtual void Load() = 0;
		GameObject* AddSceneObject(std::string name, TagName tag);
		void SetIsCleanUp();
		void SendComponentMessage(Component::Message message);
		void AttachActiveScene();
		void DetachActiveScene();
		void CleanUp();
		void Render();
		void ColliderUpdate();
		void ApplyRigidbody();
		void DebugGUI();
		void SetSelfScene(std::shared_ptr<Scene> self);
		std::string GetSceneName();
	};

	inline Scene::Scene(std::string name):name(name){
		GameObjectIndex.clear();
	}
	inline Scene::~Scene(){
		GameObjectIndex.clear();
	}
	inline void Scene::SetSelfScene(std::shared_ptr<Scene> self){
		this->self = self;
	}
	inline bool Scene::CompareName(std::string name){
		return this->name == name;
	}
	inline std::string DirectX::Scene::GetSceneName(){
		return this->name;
	}

}