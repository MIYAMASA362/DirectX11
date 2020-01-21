#pragma once

/*
設計：
・SceneManager
	ActiveSceneは必ず存在する(存在しない場合はnullptrとして例外を起こすべき)
*/

class Scene;
class GameObject;
class HierarchyUtility;

//*********************************************************************************************************************
//
//	SceneMaanger
//		Scnee管理
//
//*********************************************************************************************************************
class SceneManager final
{
	friend Scene;
private:
	//シングルインスタンス
	static SceneManager* pInstance;

	//シーン保管
	std::vector<std::shared_ptr<Scene>> _SceneArray;

	//現在のシーン
	std::weak_ptr<Scene> _ActiveScene;
	//遷移先シーン
	std::weak_ptr<Scene> _NextScene;
	//遷移フラグ
	bool _IsChangeScene;


	//コンストラクタ
	SceneManager();
	//デストラクタ
	~SceneManager();


public:

	//インスタンス取得
	static SceneManager* GetInstance() { return pInstance; };

	//インスタンス生成
	static void Create();
	//インスタンス破棄
	static void Destroy();

	//シーン生成
	std::weak_ptr<Scene> CreateScene(std::string name);

	//シーン読み込み
	void LoadScene(std::weak_ptr<Scene> scene);
	void LoadScene(std::string name);

	//現在のシーン取得
	std::weak_ptr<Scene>GetActiveScene() { return _ActiveScene; }
	//遷移先シーンの取得
	std::weak_ptr<Scene>GetNextScene() { return _NextScene; }

	void ChangeScene();

	//シーン名で取得
	std::weak_ptr<Scene>GetSceneByName(std::string SceneName);

	bool IsChangeScene() { return _IsChangeScene; };

	//現在のシーンを設定
	void AttachActiveScene(std::weak_ptr<Scene> scene);
	//遷移先を設定
	void SetIsChangeScene(std::weak_ptr<Scene> scene);
	//現在のシーンを破棄
	void DetachActiveScene();


public:
	//デバッグ表示
	void DebugGUI_ActiveScene();


};