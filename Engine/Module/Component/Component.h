#pragma once

#include<algorithm>

//*********************************************************************************************************************
//
//	Component 
//
//*********************************************************************************************************************
template<typename Type>
class Component:public IComponent
{
	friend cereal::access;
protected:
	//このComponentのインデックス
	static std::list<std::weak_ptr<Type>> ComponentIndex;

public:
	//コンストラクタ
	Component();
	//デストラクタ
	virtual ~Component();

	//ComponentIndexの取得
	static std::list<std::weak_ptr<Type>>& GetComponentIndex() { return ComponentIndex; };

	//Componentへmessage送信
	virtual void SendComponentMessage(std::string message) override {};
	//ComponentIndexから削除
	static void ReleaseComponentIndex(Type* instance);
	//ComponentIndexへ追加
	static void RegisterComponentIndex(std::shared_ptr<Type> instance);

	//ImGuiの設定
	virtual void OnDebugImGui() override;
	//削除時実行関数
	virtual void Release() override;
	//ObjectManager登録時実行関数
	virtual void Register(std::shared_ptr<Object> instance) override;

private:
	//シリアライズ
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::base_class<IComponent>(this));
	}

	//デシリアライズ
	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::base_class<IComponent>(this));
	}


};



//*********************************************************************************************************************
//
//	Component Method
//
//*********************************************************************************************************************

template<typename Type>
std::list<std::weak_ptr<Type>> Component<Type>::ComponentIndex;

//コンストラクタ
//
//
template<typename Type>
inline Component<Type>::Component()
	:
	IComponent()
{

}

//デストラクタ
//
//
template<typename Type>
inline Component<Type>::~Component()
{

}


template<typename Type>
inline void Component<Type>::ReleaseComponentIndex(Type * instance)
{
	ComponentID id = instance->GetComponentID();
	auto end = ComponentIndex.end();
	auto find = std::remove_if(
		ComponentIndex.begin(), end, [id](std::weak_ptr<IComponent> component)
	{
		return component.lock()->GetComponentID() == id;
	});
	ComponentIndex.erase(find,end);
}

//RegisterComponentIndex
//	ComponentIndexに登録する
//
template<typename Type>
inline void Component<Type>::RegisterComponentIndex(std::shared_ptr<Type> instance)
{
	ComponentIndex.push_back(instance);
}

//OnDebugImGui
//	ImGuiデバッグ表示
//
template<typename Type>
inline void Component<Type>::OnDebugImGui()
{
	IComponent::OnDebugImGui();
}

//Release
//
//
template<typename Type>
inline void Component<Type>::Release()
{
	IComponent::Release();
	ReleaseComponentIndex(static_cast<Type*>(this));
}

//Register
//
//
template<typename Type>
inline void Component<Type>::Register(std::shared_ptr<Object> instance)
{
	IComponent::Register(instance);
	RegisterComponentIndex(std::dynamic_pointer_cast<Type>(instance));
}
