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
	static std::map<ComponentID, std::weak_ptr<Type>> ComponentIndex;

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

	//ComponentIndexへ追加
	static void RegisterComponentIndex(std::shared_ptr<Type> instance);

	void ReleaseComponentIndex(std::shared_ptr<Type> instance);

	virtual void Destroy() override;

protected:
	virtual void OnDebugImGui() override;

	virtual void SendComponentMessage(std::string message) override {};

};



//*********************************************************************************************************************
//
//	Component Method
//
//*********************************************************************************************************************

template<typename Type>
std::map<ComponentID, std::weak_ptr<Type>> Component<Type>::ComponentIndex;

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
	//ComponentIndex.erase(this->GetComponentID());
}

//GetComponent
//
//
template<typename Type>
inline std::weak_ptr<Type> Component<Type>::GetComponent(EntityID entityID)
{
	for(auto component : ComponentIndex)
	{
		if (component.second.lock()->GetOwnerID() == entityID)
			return component.second;
	}
	assert(0);
	return std::weak_ptr<Type>();
}

//RegisterComponentIndex
//	ComponentIndexに登録する
//
template<typename Type>
inline void Component<Type>::RegisterComponentIndex(std::shared_ptr<Type> instance)
{
	ComponentIndex.emplace(instance->GetComponentID(), instance);
}

//ReleaseComponentIndex
//	ComponentIndexから削除する
//
template<typename Type>
inline void Component<Type>::ReleaseComponentIndex(std::shared_ptr<Type> instance)
{
	ComponentIndex.erase(instance->GetComponentID());
}

template<typename Type>
inline void Component<Type>::Destroy()
{
	IComponent::Destroy();
}

//OnDebugImGui
//	ImGuiデバッグ表示
//
template<typename Type>
inline void Component<Type>::OnDebugImGui()
{
	IComponent::OnDebugImGui();
}