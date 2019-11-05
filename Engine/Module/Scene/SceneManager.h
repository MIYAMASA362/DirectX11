#pragma once

/*
設計：
・SceneManager
	ActiveSceneは必ず存在する(存在しない場合はnullptrとして例外を起こすべき)
*/

namespace DirectX
{
	class Scene;
	class GameObject;

	using SceneID = unsigned int;

	//=== SceneManager ===========================================================
	class SceneManager
	{
		friend Scene;
	private:
		static SceneID m_SceneID;
		static std::map<SceneID, std::shared_ptr<Scene>> pSceneDictionary;

		static std::weak_ptr<Scene> pActiveScene;
		static std::weak_ptr<Scene> pNextScene;
		static bool IsChangeScene;

	private:
		static void SetIsChangeScene(std::weak_ptr<Scene> scene);
		static void AttachActiveScene(std::weak_ptr<Scene> scene);
		static void DetachActiveScene();
		static SceneID AttachID();

	public:
		SceneManager() = delete;
		~SceneManager() = delete;

		template<typename Type> static std::weak_ptr<Scene> CreateScene();
		static void Destroy();

		static void LoadScene(std::weak_ptr<Scene> scene);
		static std::weak_ptr<Scene>GetActiveScene();
		static std::weak_ptr<Scene>GetScene(SceneID id);
		static std::weak_ptr<Scene>GetSceneByName(std::string SceneName);

		static void ChangeScene();
		static void DebugGUI_ActiveScene();
	};

	//----------------------------------------------------------------------------
	template<typename Type> static std::weak_ptr<Scene> SceneManager::CreateScene()
	{
		auto AddScene = std::shared_ptr<Scene>(new Type());
		pSceneDictionary.emplace(AddScene->GetSceneID(), AddScene);
		return AddScene;
	}

	//=== Scene ==================================================================
	class Scene
	{
	private:
		const SceneID m_id;
		const std::string m_name;
		bool IsLoaded = false;
		std::vector<EntityID> Index;

	protected:
		Scene(std::string name);
		virtual ~Scene();

	public:
		std::string GetSceneName();

		GameObject* AddSceneObject(std::string name, TagName tag);
		std::weak_ptr<Scene> GetSelfScene();
		void AttachActiveScene();
		void DetachActiveScene();
		void DebugGUI();
		bool CompareName(std::string name);
		SceneID GetSceneID();

		virtual void Load() = 0;
		void UnLoad();
	};

	//----------------------------------------------------------------------------
	inline Scene::Scene(std::string name)
	:
		m_name(name),
		m_id(SceneManager::AttachID())
	{

	};
	inline Scene::~Scene()
	{
	
	};
	inline bool Scene::CompareName(std::string name)
	{
		return this->m_name == name;
	};
	inline SceneID Scene::GetSceneID()
	{
		return this->m_id;
	}
	inline std::string Scene::GetSceneName()
	{
		return this->m_name;
	};
	inline std::weak_ptr<Scene> Scene::GetSelfScene() 
	{
		return SceneManager::GetScene(this->m_id);
	};
}