#pragma once

//*********************************************************************************************************************
//
//	ComponentArray
//	�@IComponent�̔z��
//
//*********************************************************************************************************************
class ComponentList final
{
	friend class ComponentManager;
private:
	//Components
	std::list<std::weak_ptr<IComponent>> _components;


public:
	//�R���X�g���N�^
	ComponentList();

	//�f�X�g���N�^
	~ComponentList();

	//Components�֒ǉ�
	void Add(std::weak_ptr<IComponent> component);
	//Components�֒ǉ�
	void Add(ComponentList* list);
	//Component�擾
	std::weak_ptr<IComponent> Get(ComponentTypeID componentTypeID);
	//Component���폜
	void Remove(ComponentTypeID componentTypeID);
	//Component���폜
	void Remove(ComponentID componentID);
	//Components���폜
	void Release();

	//Components�̃T�C�Y
	size_t Size() { return _components.size(); };

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("Components", _components));
	}
};
