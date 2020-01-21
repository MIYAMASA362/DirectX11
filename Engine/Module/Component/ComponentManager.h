#pragma once

//Entityに付随するComponents
using EntityComponents = std::unordered_map<EntityID, std::shared_ptr<ComponentList>>;

class IEntity;

//*********************************************************************************************************************
//
//	ComponentManager
//
//*********************************************************************************************************************
class ComponentManager final
{
private:
	//インスタンス
	static ComponentManager* pInstance;

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

	static ComponentManager* GetInstance() { return pInstance; };

	//Componentに対してmessage送信
	void SendComponentMessage(std::string message);
	//特定のEntityのComponentにmessage送信
	void SendComponentMessage(std::string message, EntityID entityID);


	//EntityのComponentリストを生成
	std::weak_ptr<ComponentList> CreateComponents(IEntity* entity);
	//EntityのComponentリストを取得
	std::weak_ptr<ComponentList> GetComponents(IEntity* entity);
	//EntityのComponentsをObjectManagerの完全削除
	void DestroyComponents(IEntity* entity);
	//EntityのComponentsを解放
	void ReleaseComponents(IEntity* entity);


	//EntityにComponentを追加
	template<typename Type> 
	std::shared_ptr<Type> AddComponent(IEntity* entity);
	//EntityのComponentを取得
	template<typename Type>
	std::shared_ptr<Type> GetComponent(IEntity* entity);
	//EntityのComponentを解放
	template<typename Type>
	void ReleaseComponent(IEntity* entity);

	//全ComponentのDebug表示
	void ImGui_ComponentView(EntityID id);


};



//EntityにComponentを追加
template<typename Type>
inline std::shared_ptr<Type> ComponentManager::AddComponent(IEntity* entity)
{
	//インスタンス生成
	auto* instance = new Type(entity->GetEntityID());
	ObjectManager::GetInstance()->RegisterObject(instance);
	Component<Type>::RegisterComponentIndex(instance);

	std::shared_ptr<Type> component = std::dynamic_pointer_cast<Type>(instance->GetSelf().lock());

	entity->GetComponents()->Add(component);
	return component;
}

//EntityのComponentを取得
template<typename Type >
inline std::shared_ptr<Type> ComponentManager::GetComponent(IEntity* entity)
{
	return std::dynamic_pointer_cast<Type>(entity->GetComponents()->Get(typeid(Type).hash_code()).lock());
}

//EntityのComponentを開放
template<typename Type>
inline void ComponentManager::ReleaseComponent(IEntity* entity)
{
	entity->GetComponents()->Remove(Component<Type>::GetTypeID());
}