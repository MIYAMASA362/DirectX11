#pragma once
#include<functional>
#include"IMGUI\GUI_ImGui.h"

using EntityID = InstanceID;
using ComponentID = InstanceID;
using ComponentTypeID = size_t;

#define OBJECT_CAPACITY (60)

#define ENTITY_CAPACITY (30)

#define COMPONENTS_CAPACITY (30)

//ECSに必要なヘッダーファイルをIncludeします。
#ifndef NOT_INCLUDE_ECS_FILES

#include"Component\IComponent.h"
#include"Component\ComponentManager.h"
#include"Component\Component.h"

#include"Module\Entity\IEntity.h"
#include"Module\Entity\EntityManager.h"
#include"Module\Entity\Entity.h"

#endif //NOT_INCLUDE_ECS_FILES
