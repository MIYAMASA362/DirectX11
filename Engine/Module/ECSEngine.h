#pragma once
#include<functional>
#include"IMGUI\GUI_ImGui.h"

//Object�ŗLID
using InstanceID = unsigned int;

using EntityID = InstanceID;
using ComponentID = InstanceID;
using ComponentTypeID = size_t;

//ECS�ɕK�v�ȃw�b�_�[�t�@�C����Include���܂��B
#ifndef NOT_INCLUDE_ECS_FILES

//Object
#include"Object\Object.h"
#include"Object\ObjectManager.h"

#include"Component\IComponent.h"
#include"Component\ComponentList.h"
#include"Component\ComponentManager.h"
#include"Component\Component.h"

#include"Module\Entity\IEntity.h"
#include"Module\Entity\EntityManager.h"
#include"Module\Entity\Entity.h"

#ifndef NOT_INCLUDE_BEHAVIOUR

#include"Module\Behaviour\Behaviour.h"
#include"Module\MonoBehaviour\MonoBehaviour.h"

#endif

#endif //NOT_INCLUDE_ECS_FILES
