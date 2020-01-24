#pragma once


class Transform;
class GameObject;

//*********************************************************************************************************************
//
//	IComponent
//
//*********************************************************************************************************************
class IComponent:public Object
{
	//Componentの管理
	friend class ComponentManager;
	friend cereal::access;
private:
	//OwnerID 所有EntityのID
	EntityID _ownerId;

	//ComponentManagerで管理されているComponent
	std::weak_ptr<IComponent> _self;

	std::weak_ptr<Transform> _transform;
	std::weak_ptr<GameObject> _gameObject;

	//シリアライズ
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Object>(this),
			CEREAL_NVP(_ownerId)
		);
	}
	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Object>(this),
			_ownerId
		);
	}

public:
	//コンストラクタ
	IComponent();
	IComponent(EntityID OwnerID);
	//デストラクタ
	virtual ~IComponent();

	//コンポーネントの取得
	std::weak_ptr<IComponent> GetComponent() { return _self; };
	//OwnerIDの取得
	EntityID GetOwnerID() const { return _ownerId; };
	//ComponentTypeIDの取得
	ComponentTypeID GetComponentTypeID() { return typeid(*this).hash_code(); }
	//ComponentIDの取得
	ComponentID GetComponentID() { return GetInstanceID(); };

	//EntityのTranformへのアクセス
	std::shared_ptr<Transform> transform();
	//EntityのGameObjectへのアクセス
	std::shared_ptr<GameObject> gameObject();


protected:
	//削除時実行関数
	virtual void OnDestroy() {};

	//Componentにメッセージを送信された時の動作
	virtual void SendComponentMessage(std::string message) {};

	//ImGuiの設定
	virtual void OnDebugImGui();


};

CEREAL_REGISTER_TYPE(IComponent)