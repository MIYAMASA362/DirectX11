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
	std::vector<std::shared_ptr<Object>> _ObjectIndex;
	//Object Instance　削除インデックス
	std::stack<InstanceID> _DestroyIndex;


	//インスタンスの取得
	static ObjectManager* GetInstance() { return pInstance; }

	//インスタンス生成
	static void Create();
	//インスタンス破棄
	static void Destroy();

	//Object登録
	void RegisterObject(std::shared_ptr<Object> instance);

	//Object削除登録
	void DestroyObject(Object* object);
	//Object削除 Object::OnDestory()の実行
	void ClearnUpObject();

	//Editorデバッグ表示
	void EditorWindow();


};