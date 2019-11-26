#include"Common.h"
#include<random>

#include"Module\ECSEngine.h"

using namespace DirectX;

EntityComponents	ComponentManager::_EntityComponentIndex;
ComponentTypeIndex	ComponentManager::_ComponentTypeIndex;

void ComponentManager::Create(){
	_EntityComponentIndex.clear();
	_ComponentTypeIndex.clear();
}

void ComponentManager::Release(){
	_EntityComponentIndex.clear();
	_ComponentTypeIndex.clear();
}

//Component����ID��ݒ�
ComponentTypeID ComponentManager::AttachComponentTypeID(){
	std::random_device rand;
	return rand();
}

//Components�̐����E�擾
std::weak_ptr<Components> ComponentManager::GetOrCreateComponentIndex(ComponentTypeID componentTypeID)
{
	auto find = _ComponentTypeIndex.find(componentTypeID);
	if (find != _ComponentTypeIndex.end())
		return find->second;

	auto instance = _ComponentTypeIndex.emplace(componentTypeID,std::shared_ptr<Components>(new Components()));
	return instance.first->second;
}

//Components�̎擾
std::weak_ptr<Components> DirectX::ComponentManager::GetComponentIndex(ComponentTypeID componentTypeID)
{
	auto find = _ComponentTypeIndex.find(componentTypeID);
	if (find == _ComponentTypeIndex.end())
		return std::weak_ptr<Components>();
	return find->second;
}

//ReleaseComponents
void DirectX::ComponentManager::ReleaseComponentIndex(ComponentTypeID componentTypeID)
{
	_ComponentTypeIndex.erase(componentTypeID);
}

//Component�ɑ΂���message���M
void DirectX::ComponentManager::SendComponentMessage(std::string message)
{
	for (auto components : _EntityComponentIndex)
		for (auto component : *components.second)
			component.lock()->SendComponentMessage(message);
}

//�����Entity��Component�ɑ΂���message���M
void DirectX::ComponentManager::SendComponentMessage(std::string message, EntityID entityID)
{
	for(auto component : *_EntityComponentIndex.at(entityID))
		component.lock()->SendComponentMessage(message);
}

//
std::weak_ptr<IComponent> DirectX::ComponentManager::AddComponentIndex(std::weak_ptr<Object> object)
{
	//IComponent�̐ݒ�
	auto sptr = std::dynamic_pointer_cast<IComponent>(object.lock());
	sptr->_self = sptr;

	//EntityComponentIndex�ɒǉ�
	auto find = _EntityComponentIndex.find(sptr->GetOwnerID());
	if (find == _EntityComponentIndex.end())
		find = _EntityComponentIndex.emplace(sptr->GetOwnerID(),std::shared_ptr<Components>(new Components())).first;
	find->second->push_back(sptr);

	//ComponentTypeIndex�ɒǉ�
	_ComponentTypeIndex.at(sptr->GetComponentTypeID())->push_back(sptr);

	return sptr;
}

//Entity�̎����Ă���Components���폜
void DirectX::ComponentManager::DestroyComponents(EntityID id)
{
	for(auto component : *_EntityComponentIndex.at(id))
		component.lock()->Destroy();
}

//Entity�̎����Ă���Components���J��
void DirectX::ComponentManager::ReleaseComponents(EntityID id)
{
	_EntityComponentIndex.erase(id);
}

//Entity�̎����Ă���Components��Debug�\��
void DirectX::ComponentManager::ImGui_ComponentView(EntityID id)
{
	for (auto component : *_EntityComponentIndex.at(id))
		component.lock()->OnDebugImGui();
}
