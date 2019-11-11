#include<memory>
#include<list>
#include<Windows.h>

#include"Engine.h"

#include"DirectX\DirectXStruct.h"
#include"DirectX\DirectX.h"

#include"Transform\Transform.h"
#include"Tag\Tag.h"
#include"GameObject\GameObject.h"

using namespace DirectX;

DirectX::IComponent::IComponent(EntityID OwnerID)
	:
	_ownerId(OwnerID),
	_gameObject(GameObject::GetEntity(OwnerID))
{

}

DirectX::IComponent::~IComponent()
{
	_gameObject.reset();
}

std::shared_ptr<Transform> IComponent::transform()
{
	return Transform::GetComponent(GetOwnerID()).lock();
}

std::shared_ptr<GameObject> DirectX::IComponent::gameObject()
{
	return _gameObject.lock();
}

const InstanceID DirectX::IComponent::GetOwnerID() const
{
	return this->_ownerId;
}