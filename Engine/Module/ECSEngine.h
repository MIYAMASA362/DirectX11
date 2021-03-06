#pragma once
#include<functional>
#include"IMGUI\GUI_ImGui.h"

//Object
#include"Object\Object.h"
#include"Object\ObjectManager.h"

using EntityID = InstanceID;
using ComponentID = InstanceID;
using ComponentTypeID = size_t;

//ECSに必要なヘッダーファイルをIncludeします。
#ifndef NOT_INCLUDE_ECS_FILES

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
