#define INCLUDE_CEREAL
#include"Common.h"
#include"Module\DirectX\DirectX.h"

#define NOT_INCLUDE_BEHAVIOUR
#include"Module\ECSEngine.h"

#include"Module\Transform\Transform.h"

#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

//*********************************************************************************************************************
//
//	IComponent
//
//*********************************************************************************************************************

//IComponent
//	�R���X�g���N�^
//
IComponent::IComponent()
	:
	Object()
{

}

//IComponent
//	�R���X�g���N�^
//
IComponent::IComponent(EntityID OwnerID)
	:
	Object(),
	_Entity(EntityManager::GetInstance()->GetEntity(OwnerID).lock())
{

}

//~IComponent
//	�f�X�g���N�^
//
IComponent::~IComponent()
{
	if(_Entity) _Entity.reset();
}

ComponentTypeID IComponent::GetComponentTypeID()
{
	return typeid(*this).hash_code();
}

EntityID IComponent::GetOwnerID()
{
	return _Entity->GetEntityID();
}

//Transform
//	Entity��Transform
//
std::shared_ptr<Transform> IComponent::transform()
{
	return gameObject()->transform().lock();
}

//GameObject
//	Entity��GameObject
std::shared_ptr<GameObject> IComponent::gameObject()
{
	return std::static_pointer_cast<GameObject>(_Entity);
}

//OnDebugImGui
//
//
void IComponent::OnDebugImGui()
{
	ImGui::Text(("ID : " + std::to_string(this->GetInstanceID())).c_str());
	ImGui::Text(("OwnerID : " + std::to_string(_Entity->GetEntityID())).c_str());
}

//Destory
//	Component�̍폜����
//
void IComponent::Destroy()
{
	//�폜�o�^
	ObjectManager::GetInstance()->DestroyObject(this);
	//Entity���L�j��
	_Entity.reset();
}

void IComponent::Release()
{

}
