#pragma once

class Transform;
class GameObject;



//IComponent
//	ComponentManagerで管理されるComponent Intarface
//
class IComponent:public Object
{
	//Componentの管理
	friend class ComponentManager;
private:
	//OwnerID 所有EntityのID
	const EntityID _ownerId;
	//Entity GameObject
	std::weak_ptr<GameObject> _gameObject;


protected:
	//ComponentManagerで管理されているComponent
	std::weak_ptr<IComponent> _self;

	//Componentにメッセージを送信された時の動作
	std::function<void(std::string)> SendComponentMessage = {};
	//ImGuiの設定
	std::function<void(void)> OnDebugImGui = {};


public:
	//コンストラクタ
	IComponent(EntityID OwnerID);
	//デストラクタ
	virtual ~IComponent();

	//OwnerIDの取得
	const EntityID GetOwnerID() const { return _ownerId; };
	//ComponentTypeIDの取得
	virtual const ComponentTypeID GetComponentTypeID () const =0;
	//ComponentIDの取得
	ComponentID GetComponentID() { return GetInstanceID(); };

	//EntityのTranformへのアクセス
	std::shared_ptr<Transform> transform();
	//EntityのGameObjectへのアクセス
	std::shared_ptr<GameObject> gameObject() { return _gameObject.lock(); };


protected:
	//削除時実行関数
	virtual void OnDestroy() override = 0;


};