#pragma once
namespace DirectX
{
	using InstanceID = unsigned int;

	using EntityID = InstanceID;
	using ComponentID = InstanceID;
	using ComponentTypeID = unsigned int;

	class Object;
	class IEntity;
	class IComponent;

	using ObjectIndex = std::unordered_map<InstanceID, std::shared_ptr<Object>>;
	using DestroyIndex = std::vector<InstanceID>;

	using EntityIndex = std::unordered_map<EntityID,std::weak_ptr<IEntity>>;
	
	using ComponentList = std::list<InstanceID>;

	using Components = std::unordered_map<ComponentTypeID, std::weak_ptr<IComponent>>;
	using EntityComponents = std::unordered_map<EntityID, std::shared_ptr<Components>>;
}

#include"Object\ObjectManager.h"
#include"Object\Object.h"
#include"Entity\IEntity.h"
#include"Entity\EntityManager.h"
#include"Entity\Entity.h"
#include"Component\IComponent.h"
#include"Component\ComponentManager.h"
#include"Component\Component.h"
#include"Behaviour\Behaviour.h"
#include"MonoBehaviour\MonoBehaviour.h"