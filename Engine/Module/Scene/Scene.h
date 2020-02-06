#pragma once

//*********************************************************************************************************************
//
//	Scene
//
//*********************************************************************************************************************
class Scene : public Object
{
	friend cereal::access;
private:
	//シーン名
	std::string _name;
	//読み込みされてる
	bool IsLoaded = false;		

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

	//
	void UnLoad();

	void OrderlySceneObject(HierarchyUtility* utility);

private:
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(_name),
			CEREAL_NVP(_hierarchyUtility)
		);

		/*std::map<EntityID, ComponentList> ComponentIndex;
		auto utility = _hierarchyUtility;
		for(auto hierarchy : utility.GetHierarchyMap())
		{
			auto components = ComponentManager::GetInstance()->GetComponents(hierarchy.second.GetSelf().lock().get());
			ComponentIndex.emplace(hierarchy.first,*components.lock());
		}
		archive(CEREAL_NVP(ComponentIndex));
		*/
	}

	template<class Archive>
	void load(Archive& archive)
	{
		HierarchyUtility utility;

		archive(
			CEREAL_NVP(_name),
			utility
		);

		
		this->_hierarchyUtility = utility;
		OrderlySceneObject(&this->_hierarchyUtility);
		this->_hierarchyUtility = this->_hierarchyUtility;

		//std::map<EntityID, ComponentList> ComponentIndex;
		//archive(CEREAL_NVP(ComponentIndex));

		//for(auto hierarchy : _hierarchyUtility.GetHierarchyMap())
		//{
		//	auto gameObject = std::dynamic_pointer_cast<GameObject>(hierarchy.second.GetSelf().lock()).get();
		//	auto instance = this->CreateInstance(gameObject);
		//	this->_hierarchyUtility = this->_hierarchyUtility;

		//	auto find = ComponentIndex.find(hierarchy.first);
		//	if (find == ComponentIndex.end()) continue;
		//	for(auto component : find->second._components)
		//	{
		//		//インスタンス生成
		//		ComponentManager::GetInstance()->AddComponentInstance(instance,component.lock().get());

		//		auto obj = component.lock().get();
		//		size_t c_obj = typeid(*obj).hash_code();
		//		size_t transform = typeid(Transform).hash_code();

		//		std::string name = typeid(*obj).name();
		//		obj = obj;
		//	}
		//}
	}
};

#define STATIC_ATTRIBUTE static