#pragma once


class Transform;
class GameObject;
class IEntity;

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
	//所有者
	std::shared_ptr<IEntity> _Entity;

	//シリアライズ
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Object>(this),
			_Entity
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Object>(this),
			_Entity
		);
	}

public:
	//コンストラクタ
	IComponent();
	IComponent(EntityID OwnerID);

	//デストラクタ
	virtual ~IComponent();

	//ComponentTypeIDの取得
	ComponentTypeID GetComponentTypeID();
	//ComponentIDの取得
	ComponentID GetComponentID() { return GetInstanceID(); };

	EntityID GetOwnerID();

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

	//削除命令
	virtual void Destroy();

	//削除時実行関数
	virtual void Release();
};

CEREAL_REGISTER_TYPE(IComponent)