#pragma once

#define SCENE_OPTION_UPDATE_DEFAULT	(0x00)	//標準
#define SCENE_OPTION_UPDATE_ALL		(0x01)	//例外なくすべて更新
#define SCENE_OPTION_UPDATE_PARENT	(0x02)	//親子階層を利用した更新
#define SCENE_OPTION_UPDATE_ACTIVE	(0x04)	//アクティブのみ更新

namespace DirectX
{
	class Scene;
	class GameObject;

	/*
	設計：
		ActiveSceneは必ず存在する(存在しない場合はnullptrとして例外を起こすべき)
	*/

	//=== Scene管理 ===========================================================
	class SceneManager
	{
	private:
		static SceneManager* pInstance;
		//Sceneの保管庫
		static std::list<std::shared_ptr<Scene>> pSceneIndex;
		//現在のScene
		static std::weak_ptr<Scene> pActiveScene;
		//次のScene
		static std::weak_ptr<Scene> pNextScene;
		static bool IsChangeScene;
	private:
		SceneManager();
		~SceneManager();
	//--- Singleton -------------------------------------------------
	public:
		static void Create();			//インスタンス生成
		static void Destroy();			//インスタンス削除

	//--- Scene処理 -------------------------------------------------
	public:
		//Sceneの生成
		template<typename Type>
		static void CreateScene()
		{
			std::shared_ptr<Scene> AddScene = std::shared_ptr<Scene>(new Type());
			pSceneIndex.push_back(AddScene);
			AddScene->self = AddScene;
			return;
		};
	public:
		//Sceneの読み込み
		static void LoadScene(std::string SceneName);
		//Sceneの読み込み
		static void LoadScene(std::weak_ptr<Scene> scene);	
		//ActiveSceneへメッセージを送信
		static void RunActiveScene(Component::Message message,BYTE option = SCENE_OPTION_UPDATE_DEFAULT);
		//ActiveSceneの描画処理
		static void RunActiveScene_Render();
		//ColliderUpdate
		static void ColliderUpdate();
		//ActiveScene内オブジェクトのImGUI表示
		static void DebugGUI_ActiveScene();
		//ActiveScene内で削除指定されたオブジェクトを削除
		static void CleanUp();
		//ActiveSceneとNextSceneを切り替え
		static void ChangeScene();
		//GetActiveScene
		static std::weak_ptr<Scene>GetActiveScene();
		//GetSceneByName
		static std::weak_ptr<Scene>GetSceneByName(std::string SceneName);
		static void ApplyRigidbody();

	//--- 内部処理 --------------------------------------------------
	private:
		//SetIsChangeScene
		static void SetIsChangeScene(std::weak_ptr<Scene> scene);
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
		std::weak_ptr<Scene> self;	//自身への参照
		std::list<std::shared_ptr<GameObject>> GameObjectIndex;
	protected:
		Scene(std::string name);
		virtual ~Scene();
	public:
		//Sceneが読み込まれた時
		virtual void Load() = 0;
		//ゲームオブジェクトの生成
		GameObject* AddSceneObject(std::string name, TagName tag);
		//ゲームオブジェクトの削除処理を予約
		void SetIsCeanUp(bool IsEnable);
	};
}