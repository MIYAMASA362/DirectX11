#pragma once

class Entity;

class IComponentSystem
{
protected:
	Entity* m_CurrentEntity;

public:
	virtual void Update() = 0;
	void SetEntity(Entity* entity) { m_CurrentEntity = entity; };
};

class MoveSystem final : public IComponentSystem
{
private:

public:
	virtual void Update() override;
};