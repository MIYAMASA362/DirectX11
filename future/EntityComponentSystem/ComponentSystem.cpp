#include<map>
#include<typeinfo>

#include "Object.h"
#include"Component.h"
#include"Entity.h"
#include "ComponentSystem.h"

void MoveSystem::Update()
{
	Transform* transform = (Transform*)m_CurrentEntity->GetComponent(Transform::ComponentType());

	auto position = transform->GetPosition();
	position.x += 1.0f;
	position.y += 1.0f;

	transform->SetPosition(position);
}
