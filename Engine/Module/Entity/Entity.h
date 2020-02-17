#pragma once

//*********************************************************************************************************************
//
//	Entity
//		Type毎のEntity管理
//
//*********************************************************************************************************************
template<typename Type>
class Entity:public IEntity
{
	friend cereal::access;
protected:
	//このEntityのインスタンス配列
	static std::list<std::weak_ptr<Type>> EntityIndex;


public:
	//コンストラクタ
	Entity();
	//デストラクタ
	virtual ~Entity();

	//EntityIndexの取得
	static std::list<std::weak_ptr<Type>>& GetEntityIndex() { return EntityIndex; };

	

protected:
	//即時破棄関数
	virtual void Release() override;
	//ObjectManager登録時実行関数
	virtual void Register(std::shared_ptr<Object> instance) override;

	//EntityIndexから削除
	static void ReleaseEntityIndex(Type* instance);
	//EntityIndexへ登録
	static void RegisterEntityIndex(std::shared_ptr<Type> instance);


private:
	//シリアライズ
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::base_class<IEntity>(this));
	}

	//デシリアライズ
	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::base_class<IEntity>(this));
	}


};



template<typename Type>
std::list<std::weak_ptr<Type>> Entity<Type>::EntityIndex;

//Entity
//	コンストラクタ
//
template<typename Type>
Entity<Type>::Entity()
	:
	IEntity()
{
	
}

//~Entity
//	デストラクタ
//
template<typename Type>
Entity<Type>::~Entity()
{
	
}

//RegisterEntityIndex
//	
//
template<typename Type>
inline void Entity<Type>::RegisterEntityIndex(std::shared_ptr<Type> instance)
{
	return EntityIndex.push_back(instance);
}

//ReleaseEntityIndex
//
//
template<typename Type>
inline void Entity<Type>::ReleaseEntityIndex(Type* instance)
{
	EntityID id = instance->GetEntityID();

	auto end = EntityIndex.end();
	auto find = std::remove_if(
		EntityIndex.begin(), end, [id](std::weak_ptr<Type> entity) 
	{
		return entity.lock()->GetEntityID() == id;
	});
	EntityIndex.erase(find,end);
}

//Release
//
//
template<typename Type>
inline void Entity<Type>::Release()
{
	IEntity::Release();
	ReleaseEntityIndex(static_cast<Type*>(this));
}

template<typename Type>
inline void Entity<Type>::Register(std::shared_ptr<Object> instance)
{
	IEntity::Register(instance);
	this->RegisterEntityIndex(std::dynamic_pointer_cast<Type>(instance));
}
