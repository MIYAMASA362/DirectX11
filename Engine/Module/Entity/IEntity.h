#pragma once


using Components = std::vector<std::shared_ptr<IComponent>>;
//*********************************************************************************************************************
//
//	IEntity
//		Componentを保持
//
//*********************************************************************************************************************
class IEntity :public Object
{
	friend cereal::access;
	friend class IComponent;
private:
	//Components
	Components _Components;


public:
	//コンストラクタ
	IEntity();
	//デストラクタ
	virtual ~IEntity();

	//Entityの識別ID
	inline EntityID GetEntityID();

	//Componentsの取得
	Components& GetComponents();

	//Componentの追加
	template<typename Type>
	std::shared_ptr<Type> AddComponent();

	//Componentの取得
	template<typename Type>
	std::shared_ptr<Type> GetComponent();

	//削除宣言
	void Destroy() override;
	//Componentsへの送信
	void SendComponentMessage(std::string message);

protected:
	//即時破棄関数
	virtual void Release() override;
	//Componentの破棄
	void ReleaseComponent(IComponent* component);
	//ObjectManager登録時実行関数
	virtual void Register(std::shared_ptr<Object> instance) override;


private:
	//シリアライズ
	template<class Archive>
	void save(Archive& archive) const
	{
		cereal::base_class<Object>(this);
		archive(_Components);
	}

	//デシリアライズ
	template<class Archive>
	void load(Archive& archive)
	{
		cereal::base_class<Object>(this);
		archive(_Components);
	}


};

inline EntityID IEntity::GetEntityID()
{
	return GetInstanceID();
}

inline Components & IEntity::GetComponents()
{
	return _Components;
}


CEREAL_REGISTER_TYPE(IEntity)

template<typename Type>
inline std::shared_ptr<Type> IEntity::AddComponent()
{
	//Componentの生成
	auto component = std::shared_ptr<Type>(new Type());

	//OwnerEntityの設定
	component->SetEntity(this);

	//ObjectManagerへ登録
	ObjectManager::GetInstance()->RegisterObject(component);

	//Componentsへの追加
	this->_Components.push_back(component);

	return component;
}

template<typename Type>
inline std::shared_ptr<Type> IEntity::GetComponent()
{
	for (auto component : this->_Components)
	{
		if (std::shared_ptr<Type> result = std::dynamic_pointer_cast<Type>(component))
			return result;
	}
	return std::shared_ptr<Type>();
}
