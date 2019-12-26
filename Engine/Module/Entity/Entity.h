#pragma once

//Entity
//	
//
template<typename Type>
class Entity:public IEntity
{
protected:
	//このEntityのインスタンス配列
	static std::unordered_map<EntityID,std::weak_ptr<Type>> g_EntityIndex;

public:
	//コンストラクタ
	Entity();
	//デストラクタ
	virtual ~Entity();

	//Entityの取得
	static std::weak_ptr<Type> GetTypeEntity(EntityID id)
	{
		return g_EntityIndex.at(id);
	}
};

template<typename Type>
std::unordered_map<EntityID, std::weak_ptr<Type>> Entity<Type>::g_EntityIndex;



//Entity
//	コンストラクタ
//
template<typename Type>
Entity<Type>::Entity()
{
	
}

//~Entity
//	デストラクタ
//
template<typename Type>
Entity<Type>::~Entity()
{
	g_EntityIndex.erase(GetEntityID());
}