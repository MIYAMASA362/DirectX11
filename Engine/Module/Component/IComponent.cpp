#include"Common.h"
#include"Module\DirectX\DirectX.h"

#include"Module\ECSEngine.h"

#include"Module\Transform\Transform.h"

#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

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