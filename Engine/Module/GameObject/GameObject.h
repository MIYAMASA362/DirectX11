#pragma once

class GameObject;
class Scene;

namespace cereal
{
	class access;
}

//*********************************************************************************************************************
//
//	GameObject Entity
//
//*********************************************************************************************************************
class GameObject final :public Entity<GameObject>
{
	friend Scene;
	friend cereal::access;
private:
	//所属シーン
	Scene* _Scene;			

	//オブジェクト名
	std::string _Name;
	//タグ
	Tag _Tag;

	//削除フラグ
	bool _IsDestroy = false;
	//アクティブフラグ
	bool _IsActive = true;

	std::weak_ptr<GameObject> _gameObject;
	std::weak_ptr<Transform> _transform;

public:
	//コンストラクタ
	GameObject();
	GameObject(std::string name, Scene* scene, TagName tagName);
	GameObject(GameObject* gameObject,Scene* scene);
	//デストラクタ
	virtual ~GameObject();

	//所属シーンの取得
	Scene* GetScene() { return _Scene; };

	//オブジェクト名取得
	std::string GetName() { return _Name; };

	//タグ確認
	bool CompareTag(TagName tag) { return _Tag.Compare(tag); }

	//削除フラグ取得
	bool GetIsDestroy() { return _IsDestroy; };

	//アクティブ変更
	void SetActive(bool IsActive) { _IsActive = IsActive; }

	//親の影響を受けるか
	//	TODO : 親階層見て戻り値を決める処理を追記する
	bool GetActive() { return _IsActive; }

	//自身のみ
	bool GetActiveSelf() { return _IsActive; };

	std::weak_ptr<Transform> transform();
	std::weak_ptr<GameObject> gameObject();

	//デバッグ表示
	void OnDebugGUI();

	//シリアライズ
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Entity<GameObject>>(this),
			cereal::base_class<Object>(this),
			CEREAL_NVP(_Name),
			CEREAL_NVP(_IsDestroy),
			CEREAL_NVP(_IsActive)
		);
	}
	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Entity<GameObject>>(this),
			cereal::base_class<Object>(this),
			CEREAL_NVP(_Name),
			CEREAL_NVP(_IsDestroy),
			CEREAL_NVP(_IsActive)
		);
	}
};

CEREAL_REGISTER_TYPE(Entity<GameObject>)
CEREAL_REGISTER_TYPE(GameObject)