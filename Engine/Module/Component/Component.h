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
	static std::map<EntityID, std::weak_ptr<Type>> ComponentIndex;

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

	static void RegisterComponentIndex(IComponent* instance);

protected:
	//ComponentIndex�֒ǉ�
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
//	ImGui�f�o�b�O�\��
//
template<typename Type>
inline void Component<Type>::OnDebugImGui()
{
	IComponent::OnDebugImGui();
}