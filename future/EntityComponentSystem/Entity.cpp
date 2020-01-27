#include<map>
#include<typeinfo>
#include<algorithm>

#include"Object.h"
#include"Component.h"
#include "Entity.h"

Entity::Entity(unsigned int id)
	:
	Object(id)
{
}

Entity::~Entity()
{
	for (auto component : m_ComponentList)
		delete component.second;
	m_ComponentList.clear();
}

void Entity::AddComponent(IComponent * add)
{
	this->m_ComponentList.emplace(add->GetInstanceID(),add);
}

void Entity::RemoveComponent(IComponent * remove)
{
	this->m_ComponentList.erase(remove->GetInstanceID());
}

IComponent * Entity::GetComponent(size_t id)
{
	for(auto component : m_ComponentList)
	{
		size_t cid = component.second->GetComponentType();
		if (cid == id)
			return component.second;
	}

	return nullptr;
}
