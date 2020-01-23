#pragma once

//*********************************************************************************************************************
//
//	Component 
//
//*********************************************************************************************************************
template<typename Type>
class Component:public IComponent
{
	friend class ComponentManager;
	friend cereal::access;
protected:
	//このComponentのインデックス
	static std::map<EntityID, std::weak_ptr<Type>> ComponentIndex;

private:
	//シリアライズ
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::base_class<IComponent>(this));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::base_class<IComponent>(this));
	}

public:
	//コンストラクタ
	Component();
	Component(EntityID OwnerID);
	//デストラクタ
	virtual ~Component();

	//Entityに付属されているComponentを取得
	static std::weak_ptr<Type> GetComponent(EntityID entityID);

	//削除時関数
	virtual void OnDestroy() override {};

	static void RegisterComponentIndex(IComponent* instance);

protected:
	//ComponentIndexへ追加
	void RegisterIndex();

	virtual void OnDebugImGui() override;

	virtual void SendComponentMessage(std::string message) override {};

};



//*********************************************************************************************************************
//
//	Component Method
//
//*********************************************************************************************************************

template<typename Type>
std::map<EntityID, std::weak_ptr<Type>> Component<Type>::ComponentIndex;

//Component
//	コンストラクタ
//
template<typename Type>
inline Component<Type>::Component()
	:
	IComponent()
{

}


//Component
//	コンストラクタ
//
template<typename Type>
inline Component<Type>::Component(EntityID OwnerID)
	:
	IComponent(OwnerID)
{
	
}


//デストラクタ
//	インデックスから削除
//
template<typename Type>
inline Component<Type>::~Component()
{
	//Indexから破棄
	ComponentIndex.erase(GetOwnerID());
}

//GetComponent
//
//
template<typename Type>
inline std::weak_ptr<Type> Component<Type>::GetComponent(EntityID entityID)
{
	auto find = ComponentIndex.find(entityID);
	if (find == ComponentIndex.end()) assert(0);
	return find->second.lock();
}

template<typename Type>
inline void Component<Type>::RegisterComponentIndex(IComponent * instance)
{
	ComponentIndex.emplace(instance->GetOwnerID(), std::dynamic_pointer_cast<Type>(instance->GetSelf().lock()));
}

//RegisterIndex
//
//
template<typename Type>
inline void Component<Type>::RegisterIndex()
{
	ComponentIndex.emplace(GetOwnerID(), std::dynamic_pointer_cast<Type>(Object::GetSelf().lock()));
}

//OnDebugImGui
//	ImGuiデバッグ表示
//
template<typename Type>
inline void Component<Type>::OnDebugImGui()
{
	IComponent::OnDebugImGui();
}