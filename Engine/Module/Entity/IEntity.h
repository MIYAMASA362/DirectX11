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
	EntityID GetEntityID() { return GetInstanceID(); };
	//Entityのポインタ取得
	std::weak_ptr<IEntity> GetEntity() { return _self; };

	//Componentsへの追加
	template<typename Type> std::weak_ptr<Type> AddComponent();
	//ComponentsからComponentの取得
	template<typename Type> std::weak_ptr<Type> GetComponent();
	//Componentsの取得
	std::shared_ptr<ComponentList> GetComponents() { return _components.lock(); }
	//ComponentsからComponentの削除
	template<typename Type> void DestroyComponent();
	//Componentsの削除
	void DestroyComponents();

	//自身の破棄
	void Destroy() override;


};



//AddComponent
//	EntityのComponentsへ Type型のComponentを追加
//
template<typename Type> std::weak_ptr<Type> IEntity::AddComponent()
{
	return ComponentManager::AddComponent<Type>(this);
}

//GetComponent
//	EntityのComponentsへ Type型のComponentを取得
//
template<typename Type> std::weak_ptr<Type> IEntity::GetComponent()
{
	return ComponentManager::GetComponent<Type>(this);
}

//DestroyComponent
//
//
template<typename Type> void IEntity::DestroyComponent()
{
	ComponentManager::DestroyComponent<Type>(this);
}
