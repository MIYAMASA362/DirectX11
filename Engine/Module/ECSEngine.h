#pragma once
namespace DirectX
{
	using InstanceID = unsigned int;

	using EntityID = InstanceID;
	using ComponentID = InstanceID;
	using ComponentTypeID = unsigned int;

	class Object;		//Entity・Componentの基底クラス
	class IEntity;		//Entity	EntityManagerで管理する為の基底クラス
	class IComponent;	//Component ComponentManagerで管理する為の基底クラス

	//全オブジェクト
	using ObjectIndex = std::unordered_map<InstanceID, std::shared_ptr<Object>>;
	//削除対象
	using DestroyIndex = std::vector<InstanceID>;

	//全Entity
	using EntityIndex = std::unordered_map<EntityID,std::weak_ptr<IEntity>>;

	//Component群
	using Components = std::list<std::weak_ptr<IComponent>>;
	//Entityに付随するComponents
	using EntityComponents = std::unordered_map<EntityID, std::shared_ptr<Components>>;
	//TypeID参照
	using ComponentTypeIndex = std::unordered_map<ComponentTypeID,std::shared_ptr<Components>>;
}

#include"IMGUI\GUI_ImGui.h"

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