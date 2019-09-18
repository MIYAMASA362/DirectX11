#include<string>
#include<memory>
#include<map>

#include"Module\Object\Object.h"
#include"IComponent.h"
#include"Component.h"
#include"ComponentManager.h"

//ComponentIDのカウント
DirectX::ComponentID DirectX::ComponentManager::m_id;
//EntityにアタッチされたComponentのリスト
DirectX::EntityComponents DirectX::ComponentManager::EntityComponentIndex;
