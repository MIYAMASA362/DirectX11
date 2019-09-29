#include<string>
#include<memory>
#include<map>

#include"Module\Object\Object.h"
#include"IComponent.h"
#include"Component.h"
#include"ComponentManager.h"

using namespace DirectX;

//ComponentID�̃J�E���g
DirectX::ComponentID DirectX::ComponentManager::m_id;
//Entity�ɃA�^�b�`���ꂽComponent�̃��X�g
DirectX::EntityComponents DirectX::ComponentManager::EntityComponentIndex;

void ComponentManager::DestroyComponents(EntityID id)
{
	auto itr = EntityComponentIndex.at(id)->begin();
	auto end = EntityComponentIndex.at(id)->end();

	while (itr != end) {
		itr->second->OnDestroy();
		itr = EntityComponentIndex.at(id)->erase(itr);
	}
}