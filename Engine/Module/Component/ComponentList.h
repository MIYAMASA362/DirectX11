#pragma once

using Components = std::list<std::shared_ptr<IComponent>>;

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
private:

	//Components
	Components _Components;

public:

	//コンストラクタ
	ComponentList();
	//デストラクタ
	~ComponentList();
	
	//GetComponents Componentsの取得
	inline Components& GetComponents()
	{
		return _Components;
	}

private:
	//シリアライズ
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(_Components);
	}

	//デシリアライズ
	template<class Archive>
	void load(Archive& archive)
	{
		archive(_Components);
	}

};
