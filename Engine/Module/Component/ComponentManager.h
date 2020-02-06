#pragma once

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
	//Entity ComponentList
	static std::unordered_map<EntityID, std::shared_ptr<ComponentList>> _EntityComponentIndex;

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

	//インスタンス
	static ComponentManager* GetInstance() { return pInstance; };

	//ComponentList

	//EntityのComponentListを生成
	std::weak_ptr<ComponentList> CreateComponents(IEntity* entity);
	//EntityのComponentListの削除命令
	void DestroyComponents(IEntity* entity);
	//EntityのComponentListの破棄
	void ReleaseComponents(IEntity* entity);
	//EntityのComponentListを置き換え
	std::weak_ptr<ComponentList> SwapComponents(IEntity* entity,ComponentList* ComponentList);
	std::weak_ptr<ComponentList> SwapComponents(IEntity* entity,std::shared_ptr<ComponentList> ComponentList);

	//Component

	//EntityにComponentを追加
	template<typename Type> 
	std::shared_ptr<Type> AddComponent(IEntity* entity);
	//EntityのComponentを取得
	template<typename Type>
	std::shared_ptr<Type> GetComponent(IEntity* entity);

	//Componentに対してmessage送信
	void SendComponentMessage(std::string message);
	//特定のEntityのComponentにmessage送信
	void SendComponentMessage(std::string message, EntityID entityID);

	//全ComponentのDebug表示
	void ImGui_ComponentView(EntityID id);
};


//AddComponent
//	Componentのインスタンス生成
//	ObjectManagerへ追加
//
template<typename Type>
std::shared_ptr<Type> ComponentManager::AddComponent(IEntity* entity)
{
	auto component = std::shared_ptr<Type>(new Type(entity->GetEntityID()));
	//ObjectManagerへ追加
	ObjectManager::GetInstance()->RegisterObject(component);
	Component<Type>::RegisterComponentIndex(component);
	//Componentsへ追加
	entity->GetComponents()->AddComponent(component);

	return component;
}

//EntityのComponentを取得
template<typename Type >
std::shared_ptr<Type> ComponentManager::GetComponent(IEntity* entity)
{
	return std::dynamic_pointer_cast<Type>(entity->GetComponents()->GetComponent((ComponentTypeID)typeid(Type).hash_code()));
}