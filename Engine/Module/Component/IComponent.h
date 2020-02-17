#pragma once


class Transform;
class GameObject;
class IEntity;

//*********************************************************************************************************************
//
//	IComponent
//		データComponent
//
//*********************************************************************************************************************
class IComponent:public Object
{
	friend cereal::access;
private:
	//OwnerEntity
	IEntity* _Entity;


public:
	//コンストラクタ
	IComponent();
	//デストラクタ
	virtual ~IComponent();

	//ComponentIDの取得
	ComponentID GetComponentID() { return GetInstanceID(); };
	//Ownerの設定
	void SetEntity(IEntity* owner) { _Entity = owner; };

	//GameObject
	GameObject* gameObject();
	//Messageを送信
	virtual void SendComponentMessage(std::string message) {};

	EntityID GetOwnerID();

protected:
	//ImGuiの設定
	virtual void OnDebugImGui();
	//削除時実行関数
	virtual void Release() override;
	//ObjectManager登録時実行関数
	virtual void Register(std::shared_ptr<Object> instance) override;


private:
	//シリアライズ
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Object>(this)
		);
	}

	//デシリアライズ
	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Object>(this)
		);
	}


};

CEREAL_REGISTER_TYPE(IComponent)