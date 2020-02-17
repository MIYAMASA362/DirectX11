#pragma once

//*********************************************************************************************************************
//
//	Scene
//
//*********************************************************************************************************************
class Scene : public Object
{
	friend class SceneManager;
	friend cereal::access;
private:
	//シーン名
	std::string _name;
	//読み込みされてる
	bool IsActive = false;

	//ファイルパス
	std::string _filePath;

	//オブジェクト階層
	HierarchyUtility _hierarchyUtility;

	//コンストラクタ
	Scene();


public:
	//コンストラクタ
	Scene(std::string name, std::string path);
	//デストラクタ
	virtual ~Scene();


	//シーン名取得
	std::string GetSceneName() { return this->_name; };
	//名前
	bool CompareName(std::string name);

	//オブジェクト追加
	std::shared_ptr<GameObject> AddSceneObject(std::string name, TagName tag = TagName::Default);


	//オブジェクト削除
	void RemoveSceneObject(GameObject* gameobject);

	//アクティブ化
	void AttachActiveScene();
	//非アクティブ化
	void DetachActiveScene();

	//デバッグ表示
	void DebugGUI();

	//オブジェクト階層の取得
	HierarchyUtility* GetHierarchyUtility() { return &_hierarchyUtility; };


	//読み込み関数
	virtual void Load();
	//書き込み関数
	virtual void Save();

	virtual void Destroy() override {};

protected:
	//オブジェクト生成
	std::shared_ptr<GameObject> CreateSceneObject(std::string name,TagName tag = TagName::Default);

	//
	void UnLoad();

	void Release() {}

private:
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(_name),
			CEREAL_NVP(_hierarchyUtility)
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(_name),
			CEREAL_NVP(_hierarchyUtility)
		);

	}
};

#define STATIC_ATTRIBUTE static