#include"Common.h"
#include<random>

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Object.h"
#include"ObjectManager.h"

//ObjectIndex
//	Objectのインスタンス保持
//
ObjectIndex ObjectManager::g_ObjectIndex;

//DestroyIndex
//	ObjectIndexから削除するターゲットのIndex
//
ObjectManager::DestroyIndex ObjectManager::g_DestroyIndex;



//AttachID
//	Object に固有IDを付加する
//
InstanceID ObjectManager::AttachID()
{
	std::random_device rand;
	InstanceID id;
	do
	{
		id = rand();
	} 
	while (g_ObjectIndex.find(id) != g_ObjectIndex.end());
	
	return id;
}




//AddIndex
//	ObjectIndexへObjectを追加
//
std::weak_ptr<Object> ObjectManager::AddIndex(Object * instance)
{
	return g_ObjectIndex.emplace(instance->GetInstanceID(),std::shared_ptr<Object>(instance)).first->second;
}



//AddDestroy
//	DestroyIndexへ追加
//
void ObjectManager::AddDestroy(Object * instance)
{
	g_DestroyIndex.push_back(instance->GetInstanceID());
}

//ClearnUp
//	DestroyIndex,ObjectIndexから削除
//
void ObjectManager::ClearnUp()
{
	if (g_DestroyIndex.size() == 0) return;

	for (auto id : g_DestroyIndex)
	{
		auto itr = g_ObjectIndex.find(id);
		itr->second->OnDestroy();
		g_ObjectIndex.erase(itr);
	}
	g_DestroyIndex.clear();
}



//GetInstance
//	ObjectIndexからObjectを取得
//
std::weak_ptr<Object> ObjectManager::GetInstance(InstanceID instanceID)
{
	return g_ObjectIndex.at(instanceID);
}

//Release
//	ObjectIndexを空にする
//	OnDestroy関数の実行
//
void ObjectManager::Release()
{
	for (auto obj : g_ObjectIndex)
	{
		obj.second->OnDestroy();
	}
	
	g_DestroyIndex.clear();
	g_ObjectIndex.clear();
}
