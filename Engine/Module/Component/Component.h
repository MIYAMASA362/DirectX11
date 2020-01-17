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
	//�R���X�g���N�^
	Component(EntityID OwnerID);
	//�f�X�g���N�^
	virtual ~Component();


	//Component����ID
	static ComponentTypeID GetTypeID();

	//IComponent����TypeID�փA�N�Z�X����
	const ComponentTypeID GetComponentTypeID() const override final;



	//Entity�ɕt������Ă���Component���擾
	static std::weak_ptr<Type> GetComponent(EntityID entityID);

	//Index�̎擾
	static std::map<EntityID, std::shared_ptr<Type>>* GetComponentIndex();

	//�폜���֐�
	virtual void OnDestroy() override {};

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<IComponent>(this));
	}
	
protected:
	//����Component�̃C���f�b�N�X
	static std::map<EntityID, std::weak_ptr<Type>> ComponentIndex;

	//ComponentIndex�֒ǉ�
	void RegisterIndex();
};

template<typename Type>
std::map<EntityID, std::weak_ptr<Type>> Component<Type>::ComponentIndex;


//*********************************************************************************************************************
//
//	Component Method
//
//*********************************************************************************************************************


//�R���X�g���N�^
//
//
template<typename Type>
inline Component<Type>::Component(EntityID OwnerID)
	:
	IComponent(OwnerID)
{
	//���b�Z�[�W�ɑ΂��鏈��
	this->SendComponentMessage = [](std::string message)
	{
		
	};

	//ImGui�̕\���֐�
	this->OnDebugImGui = [this]()
	{
		if (ImGui::TreeNode(typeid(*this).name()))
		{
			ImGui::Text(("ID:" + std::to_string(this->GetInstanceID())).c_str());
			ImGui::TreePop();
		}
	};

}




//�f�X�g���N�^
//	�C���f�b�N�X����폜
//
template<typename Type>
inline Component<Type>::~Component()
{
	//Index����j��
	ComponentIndex.erase(GetOwnerID());

	//�N���[��
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
