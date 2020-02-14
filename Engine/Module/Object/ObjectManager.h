#pragma once

//*********************************************************************************************************************
//
//	ObjectManager
//		Objectの管理
//
//*********************************************************************************************************************
class ObjectManager final
{
private:
	//インスタンス
	static ObjectManager* pInstance;


	//コンストラクタ
	ObjectManager();
	//デストラクタ
	~ObjectManager();

public:
	//Object Instanceインデックス
	std::unordered_map<InstanceID, std::shared_ptr<Object>> _ObjectIndex;

	//Object Instance　削除インデックス
	std::vector<InstanceID> _DestroyIndex;
	//インスタンスの取得
	static ObjectManager* GetInstance() { return pInstance; }

	//インスタンス生成
	static void Create();
	//インスタンス破棄
	static void Destroy();

	//Object登録
	std::shared_ptr<Object> RegisterObject(std::shared_ptr<Object> object);
	//Obejctの破棄
	void ReleaseObject(Object* object);
	//登録Objectの取得
	std::shared_ptr<Object> GetObjectInstance(InstanceID id);

	//Object削除登録
	void DestroyObject(Object* object);
	//Object削除 Object::OnDestory()の実行
	void ClearnUpObject();

	//Editorデバッグ表示
	void EditorWindow();
};