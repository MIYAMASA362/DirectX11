#pragma once

namespace DirectX
{
	class Scene;

	class SceneManager
	{
		friend class Scene;
	private:
		static SceneManager* pInstance;
		static std::list<Scene*> pSceneIndex;
		static Scene* pActiveScene;
	private:
		SceneManager();
		~SceneManager();
	public:
		static void Create();
		static void Destroy();
	public:
		static void LoadScene(std::string SceneName);
		static void UnLoadScene(std::string SceneName);
		template<typename Type> static Type* CreateScene()
		{
			Type* AddScene = new Type();
			pSceneIndex.push_back(AddScene);
			return AddScene;
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
		std::list<GameObject*> pGameObjects;
	protected:
		Scene(std::string name);
		virtual ~Scene();
	private:
		void Initialize();
		void Update();
		void Render();
		void Finalize();
	public:
		GameObject* AddSceneObject(std::string name)
		{
			GameObject* object = new GameObject(name,this);
			pGameObjects.push_back(object);
			return object;
		}
	};
}