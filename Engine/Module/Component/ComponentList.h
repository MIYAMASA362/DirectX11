#pragma once

//*********************************************************************************************************************
//
//	ComponentArray
//	　IComponentの配列
//
//*********************************************************************************************************************
class ComponentList final
{
	friend class ComponentManager;
	friend cereal::access;
public:
	using Components = std::unordered_map<ComponentID, std::shared_ptr<IComponent>>;
private:

	//Components
	Components _Components;

	//シリアル化
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(_Components);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(_Components);
	}

public:

	//コンストラクタ
	ComponentList();
	//デストラクタ
	~ComponentList();
	
	//GetComponents Componentsの取得
	Components& GetComponents()
	{
		return _Components;
	}

	//Componentsへ追加
	void AddComponent(std::shared_ptr<IComponent> add);
	//Componentの取得 一番最初の物
	std::shared_ptr<IComponent>& GetComponent(ComponentTypeID id);
	//Componentの所有を破棄
	void ReleaseComponent(ComponentID componentID);
};
