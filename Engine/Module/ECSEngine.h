#pragma once

namespace DirectX
{
	using EntityID = unsigned int;
	using ComponentID = unsigned int;

	class IEntity;
	class IComponent;

	using EntityIndex = std::unordered_map<EntityID, std::shared_ptr<IEntity>>;
	using EntityList = std::list<EntityID>;

	using Components = std::unordered_map<ComponentID, std::shared_ptr<IComponent>>;
	using EntityComponents = std::unordered_map<EntityID, std::shared_ptr<Components>>;
}

#include"Object\Object.h"
#include"Entity\IEntity.h"
#include"Entity\EntityManager.h"
#include"Entity\Entity.h"
#include"Component\IComponent.h"
#include"Component\ComponentManager.h"
#include"Component\Component.h"
#include"Behaviour\Behaviour.h"
#include"MonoBehaviour\MonoBehaviour.h"