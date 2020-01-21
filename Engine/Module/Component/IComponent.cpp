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
	_ownerId(OwnerID),
	_gameObject(GameObject::GetTypeEntity(OwnerID).lock())
{

}

//~IComponent
//	デストラクタ
//
IComponent::~IComponent()
{
	_gameObject.reset();
}

//Transform
//	EntityのTransform
//
std::shared_ptr<Transform> IComponent::transform()
{
	if (_transform.expired())
		_transform = Transform::GetComponent(GetOwnerID());
	return _transform.lock();
}

//GameObject
//	EntityのGameObject
//
std::shared_ptr<GameObject> IComponent::gameObject()
{
	if (_gameObject.expired())
		_gameObject = GameObject::GetTypeEntity(this->_ownerId);
	return _gameObject.lock();
}

//OnDebugImGui
//
//
void IComponent::OnDebugImGui()
{
	ImGui::Text(("ID : " + std::to_string(this->GetInstanceID())).c_str());
	ImGui::Text(("OwnerID : " + std::to_string(_ownerId)).c_str());
}