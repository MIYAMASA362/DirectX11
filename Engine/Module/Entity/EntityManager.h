#pragma once

//EntityIndex
using EntityIndex = std::unordered_map<EntityID, std::weak_ptr<IEntity>>;

//EntityManager
//	IEntityの管理
//
class EntityManager final
{
private:
	//インスタンス
	static EntityManager* g_pInstacne;

	//全Entityのインデックス
	EntityIndex* _EntityIndex;

	//コンストラクタ
	EntityManager();
	//デストラクタ
	~EntityManager();


public:
	//EntityManagerのインスタンス生成
	static void Create();
	//EntityManagerのインスタンス生成
	static void Release();

	//EntityIndexへ追加
	static std::weak_ptr<IEntity> CreateEntity(IEntity* instance);
	//EntityIndexからEntityの削除
	static void ReleaseEntity(IEntity * instance);

	//EntityIndexからEntityの取得
	static std::weak_ptr<IEntity> GetEntity(EntityID id);


};