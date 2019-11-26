#pragma once
namespace DirectX
{
	using InstanceID = unsigned int;

	using EntityID = InstanceID;
	using ComponentID = InstanceID;
	using ComponentTypeID = unsigned int;

	class Object;		//Entity�EComponent�̊��N���X
	class IEntity;		//Entity	EntityManager�ŊǗ�����ׂ̊��N���X
	class IComponent;	//Component ComponentManager�ŊǗ�����ׂ̊��N���X

	//�S�I�u�W�F�N�g
	using ObjectIndex = std::unordered_map<InstanceID, std::shared_ptr<Object>>;
	//�폜�Ώ�
	using DestroyIndex = std::vector<InstanceID>;

	//�SEntity
	using EntityIndex = std::unordered_map<EntityID,std::weak_ptr<IEntity>>;

	//Component�Q
	using Components = std::list<std::weak_ptr<IComponent>>;
	//Entity�ɕt������Components
	using EntityComponents = std::unordered_map<EntityID, std::shared_ptr<Components>>;
	//TypeID�Q��
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