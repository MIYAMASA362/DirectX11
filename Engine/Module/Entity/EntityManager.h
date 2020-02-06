#pragma once

//*********************************************************************************************************************
//
//	EntityManager
//
//*********************************************************************************************************************
class EntityManager final
{
private:
	//インスタンス
	static EntityManager* pInstacne;

	//全Entityのインデックス
	std::unordered_map<EntityID, std::weak_ptr<IEntity>> _EntityIndex;

	//コンストラクタ
	EntityManager();
	//デストラクタ
	~EntityManager();


public:
	//EntityManagerのインスタンス生成
	static void Create();
	//EntityManagerのインスタンス生成
	static void Release();
	//インスタンスの取得
	static EntityManager* GetInstance() { return pInstacne; };

	//EntityIndexへ追加
	std::weak_ptr<IEntity> RegisterEntity(std::shared_ptr<IEntity> instance);
	void DestroyEntity(IEntity* instance);
	//EntityIndexからEntityの削除
	void ReleaseEntity(IEntity * instance);

	//EntityIndexからEntityの取得
	std::weak_ptr<IEntity> GetEntity(EntityID id);


};