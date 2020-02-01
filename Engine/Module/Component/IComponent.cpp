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
//	コンストラクタ
//
IComponent::IComponent()
	:
	Object()
{

}

//IComponent
//	コンストラクタ
//
IComponent::IComponent(EntityID OwnerID)
	:
	Object(),
	_OwnerID(OwnerID),
	_Entity(EntityManager::GetInstance()->GetEntity(OwnerID).lock())
{
	ObjectManager::GetInstance()->RegisterObject(this);
}

//~IComponent
//	デストラクタ
//
IComponent::~IComponent()
{
	if(_Entity) _Entity.reset();
}

ComponentTypeID IComponent::GetComponentTypeID()
{
	return typeid(*this).hash_code();
}

//Transform
//	EntityのTransform
//
std::shared_ptr<Transform>& IComponent::transform()
{
	return gameObject()->transform().lock();
}

//GameObject
//	EntityのGameObject
std::shared_ptr<GameObject>& IComponent::gameObject()
{
	return std::dynamic_pointer_cast<GameObject>(_Entity);
}

//OnDebugImGui
//
//
void IComponent::OnDebugImGui()
{
	ImGui::Text(("ID : " + std::to_string(this->GetInstanceID())).c_str());
	ImGui::Text(("OwnerID : " + std::to_string(_OwnerID)).c_str());
}