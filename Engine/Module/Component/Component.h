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
	//����Component�̃C���f�b�N�X
	static std::list<std::weak_ptr<Type>> ComponentIndex;

public:
	//�R���X�g���N�^
	Component();
	//�f�X�g���N�^
	virtual ~Component();

	//ComponentIndex�̎擾
	static std::list<std::weak_ptr<Type>>& GetComponentIndex() { return ComponentIndex; };

	//Component��message���M
	virtual void SendComponentMessage(std::string message) override {};
	//ComponentIndex����폜
	static void ReleaseComponentIndex(Type* instance);
	//ComponentIndex�֒ǉ�
	static void RegisterComponentIndex(std::shared_ptr<Type> instance);

	//ImGui�̐ݒ�
	virtual void OnDebugImGui() override;
	//�폜�����s�֐�
	virtual void Release() override;
	//ObjectManager�o�^�����s�֐�
	virtual void Register(std::shared_ptr<Object> instance) override;

private:
	//�V���A���C�Y
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::base_class<IComponent>(this));
	}

	//�f�V���A���C�Y
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

//�R���X�g���N�^
//
//
template<typename Type>
inline Component<Type>::Component()
	:
	IComponent()
{

}

//�f�X�g���N�^
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
//	ComponentIndex�ɓo�^����
//
template<typename Type>
inline void Component<Type>::RegisterComponentIndex(std::shared_ptr<Type> instance)
{
	ComponentIndex.push_back(instance);
}

//OnDebugImGui
//	ImGui�f�o�b�O�\��
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
