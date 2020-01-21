#pragma once

//*********************************************************************************************************************
//
//	IEntity
//
//*********************************************************************************************************************
class IEntity :public Object
{
	friend class EntityManager;
	friend cereal::access;
private:
	//EntityManagerで管理されているInstanceへのアクセス
	std::weak_ptr<IEntity> _self;
	//ComponentManagerで管理されているComponentsへのアクセス
	std::weak_ptr<ComponentList> _components;

	template<class Archive>
	void save(Archive& archive)const
	{
		cereal::base_class<Object>(this);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		cereal::base_class<Object>(this);
	}

public:
	//コンストラクタ
	IEntity();
	IEntity(std::shared_ptr<IEntity> sptr,std::shared_ptr<ComponentList> components);

	//デストラクタ
	virtual ~IEntity();

	//Entityの識別ID
	EntityID GetEntityID() { return GetInstanceID();};

	//Entityのポインタ取得
	std::weak_ptr<IEntity> GetEntity() { return _self;};

	//Componentsへの追加
	template<typename Type> 
	std::weak_ptr<Type> AddComponent() { return ComponentManager::GetInstance()->AddComponent<Type>(this);}

	//ComponentsからComponentの取得
	template<typename Type>
	std::weak_ptr<Type> GetComponent() { return ComponentManager::GetInstance()->GetComponent<Type>(this); }

	//Componentsの取得
	std::shared_ptr<ComponentList> GetComponents() { return _components.lock();}

	//ComponentsからComponentの削除
	template<typename Type> 
	void DestroyComponent() { ComponentManager::GetInstance()->DestroyComponent<Type>(this); }

	//Componentsの削除
	void DestroyComponents() { ComponentManager::GetInstance()->DestroyComponents(this); }

	//ObjectManagerを通した破棄
	void Destroy() override;


};

CEREAL_REGISTER_TYPE(IEntity)