#pragma once

namespace DirectX
{
	class Scene;
	class GameObject;

	class SceneManager
	{
	private:
		static SceneManager* pInstance;
		static std::list<std::shared_ptr<Scene>> pSceneIndex;
		static std::weak_ptr<Scene> pActiveScene;
	private:
		SceneManager();
		~SceneManager();
	public:
		//Singleton
		static void Create();
		static void Destroy();
		//Scene
		static void LoadScene(std::string SceneName);
		static void UnLoadScene(std::string SceneName);
		template<typename Type> static void CreateScene()
		{
			std::shared_ptr<Scene> AddScene = std::shared_ptr<Scene>(new Type());
			pSceneIndex.push_back(AddScene);
			AddScene->self = AddScene;
			return;
		};
		static Scene* GetScene(std::string SceneName);
		static void RunActiveScene(Component::Message message);
		static void OnTriggerUpdate();
		static void OnCollisionUpdate();
		static void DebugGUI_ActiveScene();
	public:
		static void CleanUp();
	};

	class Scene 
	{
		friend class SceneManager;
	private:
		const std::string name;
		bool IsLoaded = false;
		bool IsCeanUp = false;
	private:
		std::weak_ptr<Scene> self;	//é©êgÇ÷ÇÃéQè∆
		std::list<std::shared_ptr<GameObject>> GameObjectIndex;
	protected:
		Scene(std::string name);
		virtual ~Scene();
	public:
		GameObject* AddSceneObject(std::string name, TagName tag);
		GameObject* AddSceneObject(std::string name);
		void SetIsCeanUp(bool IsEnable) { this->IsCeanUp = IsEnable; };
		
	};

}