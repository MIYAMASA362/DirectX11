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
	//����Component�̃C���f�b�N�X
	static std::map<ComponentID, std::weak_ptr<Type>> ComponentIndex;

private:
	//�V���A���C�Y
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
	//�R���X�g���N�^
	Component();
	Component(EntityID OwnerID);
	//�f�X�g���N�^
	virtual ~Component();

	//Entity�ɕt������Ă���Component���擾
	static std::weak_ptr<Type> GetComponent(EntityID entityID);

	//�폜���֐�
	virtual void OnDestroy() override {};

	//ComponentIndex�֒ǉ�
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
//	�R���X�g���N�^
//
template<typename Type>
inline Component<Type>::Component()
	:
	IComponent()
{

}


//Component
//	�R���X�g���N�^
//
template<typename Type>
inline Component<Type>::Component(EntityID OwnerID)
	:
	IComponent(OwnerID)
{

}


//�f�X�g���N�^
//	�C���f�b�N�X����폜
//
template<typename Type>
inline Component<Type>::~Component()
{
	//Index����j��
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
//	ComponentIndex�ɓo�^����
//
template<typename Type>
inline void Component<Type>::RegisterComponentIndex(std::shared_ptr<Type> instance)
{
	ComponentIndex.emplace(instance->GetComponentID(), instance);
}

//ReleaseComponentIndex
//	ComponentIndex����폜����
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
//	ImGui�f�o�b�O�\��
//
template<typename Type>
inline void Component<Type>::OnDebugImGui()
{
	IComponent::OnDebugImGui();
}