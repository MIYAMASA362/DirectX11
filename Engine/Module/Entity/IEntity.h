#pragma once

//IEntity
//	Entityの基底クラス
//	EntityManagerによって管理されるインターフェイス
//
class IEntity :public Object
{
	friend class EntityManager;
private:
	//EntityManagerで管理されているInstanceへのアクセス
	std::weak_ptr<IEntity> _self;
	//ComponentManagerで管理されているComponentsへのアクセス
	std::weak_ptr<ComponentList> _components;


public:
	//コンストラクタ
	IEntity();
	//デストラクタ
	virtual ~IEntity();

	//Entityの識別ID
	EntityID GetEntityID() 
	{ 
		return GetInstanceID();
	};

	//Entityのポインタ取得
	std::weak_ptr<IEntity> GetEntity() 
	{ 
		return _self;
	};

	//Componentsへの追加
	template<typename Type> std::weak_ptr<Type> AddComponent()
	{ 
		return ComponentManager::AddComponent<Type>(this);
	}

	//ComponentsからComponentの取得
	template<typename Type> std::weak_ptr<Type> GetComponent()
	{ 
		return ComponentManager::GetComponent<Type>(this); 
	}

	//Componentsの取得
	std::shared_ptr<ComponentList> GetComponents() 
	{ 
		return _components.lock();
	}

	//ComponentsからComponentの削除
	template<typename Type> void DestroyComponent()
	{
		ComponentManager::DestroyComponent<Type>(this); 
	}

	//Componentsの削除
	void DestroyComponents()
	{ 
		ComponentManager::DestroyComponents(this);
	}

	//ObjectManagerを通した破棄
	void Destroy() override;


};
