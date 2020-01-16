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
private:
	//Components
	std::list<std::weak_ptr<IComponent>> _components;


public:
	//コンストラクタ
	ComponentList();

	//デストラクタ
	~ComponentList();

	//Componentsへ追加
	void Add(std::weak_ptr<IComponent> component);
	//Componentsへ追加
	void Add(ComponentList* list);
	//Component取得
	std::weak_ptr<IComponent> Get(ComponentTypeID componentTypeID);
	//Componentを削除
	void Remove(ComponentTypeID componentTypeID);
	//Componentを削除
	void Remove(ComponentID componentID);
	//Componentsを削除
	void Release();

	//Componentsのサイズ
	size_t Size() { return _components.size(); };

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("Components", _components));
	}
};
