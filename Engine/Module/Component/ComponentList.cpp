#include"Common.h"

#define NOT_INCLUDE_ECS_FILES
#include"Module\ECSEngine.h"

#include"Module\Object\Object.h"
#include"Module\Object\ObjectManager.h"

#include"IComponent.h"
#include"ComponentList.h"

//*********************************************************************************************************************
//
//	ComponentList
//
//*********************************************************************************************************************

//ComponentList
//	�R���X�g���N�^
//
ComponentList::ComponentList()
{

}

//~ComponentList
//	�f�X�g���N�^
//
ComponentList::~ComponentList()
{
	this->_Components.clear();
}