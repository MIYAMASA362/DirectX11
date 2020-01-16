#pragma once

class GameObject;
class Scene;

//*********************************************************************************************************************
//
//	GameObject Entity
//
//*********************************************************************************************************************
class GameObject final:public Entity<GameObject>
{
	friend Scene;
private:
	Scene* const scene;			//対応シーン

	const std::string name;		//オブジェクト名
	const Tag tag;				//タグ

	bool IsDestroy = false;		//削除フラグ
	bool IsActive  = true;		//アクティブフラグ

	std::weak_ptr<GameObject> _gameObject;
	std::weak_ptr<Transform> _transform;

public:
	//コンストラクタ
	GameObject(std::string name,Scene* scene, TagName tagName);
	GameObject() :GameObject("Hoge",,) {}
	//デストラクタ
	virtual ~GameObject();

	void SetActive(bool IsActive);	//アクティブ変更
	bool GetActive();				//親の影響を受けるか
	
	bool CompareTag(TagName tag);	//タグ確認

	bool GetIsDestroy();		//削除フラグ取得
	bool GetActiveSelf();		//自身のみ
	Scene* GetScene();

	std::weak_ptr<Transform> transform();
	std::weak_ptr<GameObject> gameObject();

	void OnDebugGUI();
	std::string GetName();

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(name),
			CEREAL_NVP(IsDestroy),
			CEREAL_NVP(IsActive)
		);

		auto components = ComponentManager::GetComponents(this).lock();
		archive(
			CEREAL_NVP(components)
		);
	}
};

CEREAL_REGISTER_TYPE(GameObject)

CEREAL_REGISTER_POLYMORPHIC_RELATION(Entity<GameObject>,GameObject)