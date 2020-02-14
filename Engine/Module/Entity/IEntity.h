#pragma once

//*********************************************************************************************************************
//
//	IEntity
//
//*********************************************************************************************************************
class IEntity :public Object
{
	friend cereal::access;
	friend class ComponentManager;
private:
	//Components 所持しているComponent群への参照
	std::weak_ptr<ComponentList> _ComponentList;

public:
	//コンストラクタ
	IEntity();
	//デストラクタ
	virtual ~IEntity();

	//Entityの識別ID
	EntityID GetEntityID() 
	{ 
		return GetInstanceID(); 
	}
	//Componentsの取得
	std::shared_ptr<ComponentList> GetComponents() 
	{
		return _ComponentList.lock();
	}

	//Componentの追加
	template<typename Type> 
	std::weak_ptr<Type> AddComponent() 
	{
		return ComponentManager::GetInstance()->AddComponent<Type>(this);
	}
	//Componentの取得
	template<typename Type>
	std::weak_ptr<Type> GetComponent() 
	{
		return ComponentManager::GetInstance()->GetComponent<Type>(this);
	}

	//削除関数
	void Destroy() override;

protected:
	//即時破棄関数
	virtual void Release() override;

private:
	//シリアライズ
	template<class Archive>
	void save(Archive& archive) const
	{
		cereal::base_class<Object>(this);
		archive(_ComponentList.lock());
	}

	//デシリアライズ
	template<class Archive>
	void load(Archive& archive)
	{
		std::shared_ptr<ComponentList> list;
		cereal::base_class<Object>(this);
		archive(list);
		_ComponentList = ComponentManager::GetInstance()->SwapComponents(this, list);
	}

};

CEREAL_REGISTER_TYPE(IEntity)