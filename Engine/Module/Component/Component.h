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
	
public:
	Component() :Component(0) {};
	//コンストラクタ
	Component(EntityID OwnerID);
	//デストラクタ
	virtual ~Component();


	//Component毎のID
	static ComponentTypeID GetTypeID();

	//IComponentからTypeIDへアクセスする
	const ComponentTypeID GetComponentTypeID() const override final;



	//Entityに付属されているComponentを取得
	static std::weak_ptr<Type> GetComponent(EntityID entityID);

	//Indexの取得
	static std::map<EntityID, std::shared_ptr<Type>>* GetComponentIndex();

	//削除時関数
	virtual void OnDestroy() override {};

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<IComponent>(this));
	}
	
protected:
	//このComponentのインデックス
	static std::map<EntityID, std::weak_ptr<Type>> ComponentIndex;

	//ComponentIndexへ追加
	void RegisterIndex();
};

template<typename Type>
std::map<EntityID, std::weak_ptr<Type>> Component<Type>::ComponentIndex;


//*********************************************************************************************************************
//
//	Component Method
//
//*********************************************************************************************************************


//コンストラクタ
//
//
template<typename Type>
inline Component<Type>::Component(EntityID OwnerID)
	:
	IComponent(OwnerID)
{
	//メッセージに対する処理
	this->SendComponentMessage = [](std::string message)
	{
		
	};

	//ImGuiの表示関数
	this->OnDebugImGui = [this]()
	{
		if (ImGui::TreeNode(typeid(*this).name()))
		{
			ImGui::Text(("ID:" + std::to_string(this->GetInstanceID())).c_str());
			ImGui::TreePop();
		}
	};

}




//デストラクタ
//	インデックスから削除
//
template<typename Type>
inline Component<Type>::~Component()
{
	//Indexから破棄
	ComponentIndex.erase(GetOwnerID());

	//クリーン
	/*if (!_Index.expired()) {
		_Index.lock()->remove_if([](std::weak_ptr<IComponent> obj) {
			return obj.expired();
		});
	}*/

}

//GetComponent
//
//
template<typename Type>
inline std::weak_ptr<Type> Component<Type>::GetComponent(EntityID entityID)
{
	return ComponentIndex.find(entityID)->second;
}

//GetComponentIndex
//
//
template<typename Type>
inline std::map<EntityID, std::shared_ptr<Type>> * Component<Type>::GetComponentIndex()
{
	return &ComponentIndex;
}

template<typename Type>
inline void Component<Type>::RegisterIndex()
{
	ComponentIndex.emplace(GetOwnerID(), std::dynamic_pointer_cast<Type>(Object::_self.lock()));
}


//GetTypeID
//
//
template<typename Type>
inline ComponentTypeID Component<Type>::GetTypeID()
{
	return typeid(Type).hash_code();
}


//GetComponentTypeID
//
//
template<typename Type>
inline const ComponentTypeID Component<Type>::GetComponentTypeID() const
{
	return GetTypeID();
}
