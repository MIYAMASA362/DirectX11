#pragma once

class Entity : public Object
{
private:
	std::map<unsigned int,IComponent*> m_ComponentList;
public:
	Entity(unsigned int id);
	virtual ~Entity();

	void AddComponent(IComponent* add);
	void RemoveComponent(IComponent* remove);

	IComponent* GetComponent(size_t id);
};