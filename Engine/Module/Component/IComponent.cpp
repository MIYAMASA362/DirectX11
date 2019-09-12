#include<memory>
#include"../ECSEngine.h"

using namespace DirectX;

void IComponent::SetEnable(bool enable)
{
	this->m_IsEnable = enable;
}

bool IComponent::GetEnable()
{
	return this->m_IsEnable;
}

EntityID IComponent::GetOwnerID()
{
	return this->m_OwnerId;
}

std::weak_ptr<Transform> IComponent::transform()
{
	return ComponentManager::GetComponent<Transform>(this->m_OwnerId);
}

std::weak_ptr<GameObject> DirectX::IComponent::gameObject()
{
	return Entity<GameObject>::GetEntity(this->m_OwnerId);
}

void IComponent::OnDestroy()
{

}