#include<string>
#include<memory>
#include<map>

#include"Module\Object\Object.h"
#include"IComponent.h"
#include"Component.h"
#include"ComponentManager.h"

//ComponentID�̃J�E���g
DirectX::ComponentID DirectX::ComponentManager::m_id;
//Entity�ɃA�^�b�`���ꂽComponent�̃��X�g
DirectX::EntityComponents DirectX::ComponentManager::EntityComponentIndex;
