#pragma once

//Entityに付随するComponents
using EntityComponents = std::unordered_map<EntityID, std::shared_ptr<ComponentList>>;

class IEntity;

//ComponentManager
//	EntityとComponentとの関わりを管理
//
class ComponentManager final
{
private:
	//インスタンス
	static ComponentManager* g_pInstance;

	//Entityに付加されたComponentsのインデックス
	EntityComponents _EntityComponentIndex;


private:
	//コンストラクタ
	ComponentManager();
	//デストラクタ
	~ComponentManager();


public:
	//インスタンス生成
	static void Create();
	//インスタンス破棄
	static void Release();


	//Componentに対してmessage送信
	static void SendComponentMessage(std::string message);
	//特定のEntityのComponentにmessage送信
	static void SendComponentMessage(std::string message, EntityID entityID);


	//EntityのComponentリストを生成
	static std::weak_ptr<ComponentList> CreateComponents(IEntity* entity);
	//EntityのComponentリストを取得
	static std::weak_ptr<ComponentList> GetComponents(IEntity* entity);
	//EntityのComponentsをObjectManagerの完全削除
	static void DestroyComponents(IEntity* entity);
	//EntityのComponentsを解放
	static void ReleaseComponents(IEntity* entity);


	//EntityにComponentを追加
	template<typename Type>  static std::shared_ptr<Type> AddComponent(IEntity* entity);
	//EntityのComponentを取得
	template<typename Type>  static std::shared_ptr<Type> GetComponent(IEntity* entity);
	//EntityのComponentを解放
	template<typename Type> static void ReleaseComponent(IEntity* entity);

	//全ComponentのDebug表示
	static void ImGui_ComponentView(EntityID id);


};



//EntityにComponentを追加
template<typename Type>
inline std::shared_ptr<Type> ComponentManager::AddComponent(IEntity* entity)
{
	//インスタンス生成
	Object* instance = new Type(entity->GetEntityID());
	std::shared_ptr<Type> component = std::dynamic_pointer_cast<Type>(ObjectManager::GetInstance(instance->GetInstanceID()).lock());
	entity->GetComponents()->Add(component);
	return component;
}

//EntityのComponentを取得
template<typename Type >
inline std::shared_ptr<Type> ComponentManager::GetComponent(IEntity* entity)
{
	return std::dynamic_pointer_cast<Type>(entity->GetComponents()->Get(Component<Type>::GetTypeID()).lock());
}

//EntityのComponentを開放
template<typename Type>
inline void ComponentManager::ReleaseComponent(IEntity* entity)
{
	entity->GetComponents()->Remove(Component<Type>::GetTypeID());
}