#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\ECSEngine.h"

#include"Module\Transform\Transform.h"

#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

using namespace DirectX;

DirectX::IComponent::IComponent(EntityID OwnerID)
:
	m_ownerId(OwnerID)
{

}

DirectX::IComponent::~IComponent()
{
	ComponentManager::ReleaseComponent<Transform>(this->GetOwnerID());
}

std::weak_ptr<Transform> IComponent::transform()
{
	return Transform::GetComponent(this->GetOwnerID());
}

std::weak_ptr<GameObject> DirectX::IComponent::gameObject()
{
	return GameObject::GetEntity(this->GetOwnerID());
}