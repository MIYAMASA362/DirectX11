#pragma once

namespace DirectX
{
	class Scene;

	class SceneManager
	{
		friend class Scene;
	private:
		static SceneManager* pInstance;
		static std::list<std::shared_ptr<Scene>> pSceneIndex;
		static std::weak_ptr<Scene> pActiveScene;
	private:
		SceneManager();
		~SceneManager();
	public:
		static void Create();
		static void Destroy();
	public:
		static void LoadScene(std::string SceneName);
		static void UnLoadScene(std::string SceneName);
		template<typename Type> static void CreateScene()
		{
			std::shared_ptr<Scene> AddScene = std::shared_ptr<Scene>(new Type());
			pSceneIndex.push_back(AddScene);
			return;
		};
		static Scene* GetScene(std::string SceneName);
	public:
		static void Initialize();
		static void Update();
		static void Render();
		static void Finalize();
	};

	class Scene
	{
		friend class SceneManager;
	private:
		bool IsLoaded = false;
		const std::string name;
	private:
		std::list<std::shared_ptr<GameObject>> GameObjectIndex;
	protected:
		Scene(std::string name);
		virtual ~Scene();
	private:
		void Initialize();
		void Update();
		void Render();
		void Finalize();
	public:
		GameObject* AddSceneObject(std::string name,TagManager::TagName tag)
		{
			std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject(name, this,tag));
			GameObjectIndex.push_back(object);
			return object.get();
		}

		GameObject* AddSceneObject(std::string name) 
		{ 
			return AddSceneObject(name, TagManager::Default); 
		}

		void RemoveObject(GameObject* destroyObject)
		{
			for(std::shared_ptr<GameObject> gameObject:GameObjectIndex)
			{
				if (gameObject._Get() != destroyObject) continue;
				GameObjectIndex.remove(gameObject);
				return;
			}
		}
	};
}