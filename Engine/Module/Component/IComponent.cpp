#include<memory>
#include<map>
#include<vector>
#include<list>

#include<d3d11.h>
#include<DirectXMath.h>

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\ECSEngine.h"

#include"Module\Transform\Transform.h"

#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

using namespace DirectX;

DirectX::IComponent::IComponent(EntityID OwnerID)
:
	m_OwnerId(OwnerID)
{

}

void IComponent::SetEnable(bool enable)
{
	this->m_IsEnable = enable;
}

bool IComponent::GetEnable()
{
	return this->m_IsEnable;
}

EntityID DirectX::IComponent::GetOwnerID()
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