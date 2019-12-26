#pragma once

//全オブジェクト
using ObjectIndex = std::unordered_map<InstanceID, std::shared_ptr<Object>>;

//ObjectManager
//	ゲーム内のObjectを管理する
//
class ObjectManager
{
private:
	//Object Instanceインデックス
	static ObjectIndex g_ObjectIndex;

	//Objectの削除用インデックス
	using DestroyIndex = std::vector<InstanceID>;
	//Object Instance　削除インデックス
	static DestroyIndex g_DestroyIndex;


public:
	//Object に固有IDを付加する
	static InstanceID AttachID();

	//g_ObjectIndexに追加する
	static std::weak_ptr<Object> AddIndex(Object* instance);

	//g_DestroyIndexに追加する
	static void AddDestroy(Object* instance);
	//g_DestroyIndex内のオブジェクトを削除する
	static void ClearnUp();

	//g_ObjectIndexからIndexの取得
	static std::weak_ptr<Object> GetInstance(InstanceID instanceID);
	
	//g_ObjectIndexを削除
	static void Release();



};