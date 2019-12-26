#include"Common.h"
#include"Module\DirectX\DirectX.h"

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Object\Object.h"
#include"Module\Object\ObjectManager.h"

#include"Module\Component\IComponent.h"
#include"Module\Component\ComponentList.h"
#include"Module\Component\ComponentManager.h"
#include"Module\Component\Component.h"

#include"Module\Entity\IEntity.h"
#include"Module\Entity\EntityManager.h"
#include"Module\Entity\Entity.h"

#include"Module\Transform\Transform.h"

#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

//IComponent
//	コンストラクタ
//
IComponent::IComponent(EntityID OwnerID)
	:
	_ownerId(OwnerID),
	_gameObject(GameObject::GetEntity(OwnerID))
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
	return Transform::GetComponent(GetOwnerID()).lock();
}