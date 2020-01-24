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
private:

	//シリアル化
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			_components
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			_components
		);
	}

public:
	//Components
	std::list<std::weak_ptr<IComponent>> _components;

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

};
