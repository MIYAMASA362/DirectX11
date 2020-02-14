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
	static std::unordered_map<EntityID,std::weak_ptr<Type>> EntityIndex;

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::base_class<IEntity>(this));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::base_class<IEntity>(this));
	}

public:
	//コンストラクタ
	Entity();
	//デストラクタ
	virtual ~Entity();

	//Entityの取得
	static std::weak_ptr<Type> GetTypeEntity(EntityID id);

	//EntityIndexへ登録
	static std::weak_ptr<Type> RegisterEntityIndex(std::shared_ptr<Type> instance);
	//EntityIndexから削除
	static void DestroyEntityIndex(Type* instance);

protected:

	virtual void Release();
};




template<typename Type>
std::unordered_map<EntityID, std::weak_ptr<Type>> Entity<Type>::EntityIndex;

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

template<typename Type>
inline std::weak_ptr<Type> Entity<Type>::GetTypeEntity(EntityID id)
{
	auto find = EntityIndex.find(id);
	if (find == EntityIndex.end()) assert(0);
	return find->second;
}

template<typename Type>
inline std::weak_ptr<Type> Entity<Type>::RegisterEntityIndex(std::shared_ptr<Type> instance)
{
	return EntityIndex.emplace(instance->GetEntityID(),instance).first->second;
}

template<typename Type>
inline void Entity<Type>::DestroyEntityIndex(Type * instance)
{
	EntityIndex.erase(instance->GetEntityID());
}

template<typename Type>
inline void Entity<Type>::Release()
{
	IEntity::Release();
	EntityIndex.erase(this->GetEntityID());
}
